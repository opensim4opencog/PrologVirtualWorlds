/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:38 2000
 */
/* Compiler settings for avprof.idl:
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

#ifndef __avprof_h__
#define __avprof_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IAvatarProfile_FWD_DEFINED__
#define __IAvatarProfile_FWD_DEFINED__
typedef interface IAvatarProfile IAvatarProfile;
#endif 	/* __IAvatarProfile_FWD_DEFINED__ */


#ifndef __IAvatarProfileData_FWD_DEFINED__
#define __IAvatarProfileData_FWD_DEFINED__
typedef interface IAvatarProfileData IAvatarProfileData;
#endif 	/* __IAvatarProfileData_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "vwprop.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_avprof_0000 */
/* [local] */ 

#define VWAVATARPROFILE_E_INVALIDKEYMAP				MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0380)
#define VWAVATARPROFILE_E_INVALIDBANNEDOBJECTLIST	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0381)
#define VWAVATARPROFILE_E_KEYNOTEXIST				MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0382)
#define VWAVATARPROFILE_E_KEYEXISTS					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0383)
#define VWAVATARPROFILE_E_BANNEDOBJECT				MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0384)
#define VWAVATARPROFILE_E_PERMISSIONDENIED			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0385)
#define VWAVATARPROFILE_E_INVALIDOWNER				MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0386)


extern RPC_IF_HANDLE __MIDL_itf_avprof_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_avprof_0000_v0_0_s_ifspec;

#ifndef __IAvatarProfile_INTERFACE_DEFINED__
#define __IAvatarProfile_INTERFACE_DEFINED__

/* interface IAvatarProfile */
/* [unique][helpstring][dual][uuid][object] */ 

#define	DISPID_AVATARPROFILE_ADDKEY	( 1 )

#define	DISPID_AVATARPROFILE_KEYVALUE	( 2 )

#define	DISPID_AVATARPROFILE_KEYGRAPHIC	( 3 )

#define	DISPID_AVATARPROFILE_KEYOWNER	( 4 )

#define	DISPID_AVATARPROFILE_KEYACCESS	( 5 )

#define	DISPID_AVATARPROFILE_REMOVEKEY	( 6 )

#define	DISPID_AVATARPROFILE_VERIFYKEY	( 7 )

#define	DISPID_AVATARPROFILE_AVATAR	( 8 )

#define	DISPID_AVATARPROFILE_BANOBJECT	( 9 )

#define	DISPID_AVATARPROFILE_UNBANOBJECT	( 10 )

#define	DISPID_AVATARPROFILE_WRITETODISK	( 11 )

#define	DISPID_AVATARPROFILE_READFROMDISK	( 12 )

#define	DISPID_AVATARPROFILE_KEYS	( 14 )

#define	DISPID_AVATARPROFILE_RESET	( 15 )


EXTERN_C const IID IID_IAvatarProfile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D4E7472B-2953-11D1-AFB6-00C04FB915AA")
    IAvatarProfile : public IObjectProperty
    {
    public:
        virtual /* [helpstring][id][restricted] */ HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Keys( 
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppKeyList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsValid( 
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbValid) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddKey( 
            /* [in] */ BSTR bstrKey,
            /* [in] */ VARIANT varKeyVal,
            /* [optional][in] */ VARIANT varStrGraphic,
            /* [optional][in] */ VARIANT varAccess,
            /* [optional][in] */ VARIANT varOwner) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_KeyValue( 
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarKeyVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_KeyValue( 
            /* [in] */ BSTR bstrKey,
            /* [in] */ VARIANT varKeyVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_KeyGraphic( 
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrGraphic) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_KeyGraphic( 
            /* [in] */ BSTR bstrKey,
            /* [in] */ BSTR bstrGraphic) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_KeyOwner( 
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppOwner) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_KeyAccess( 
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ short __RPC_FAR *sAccess) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_KeyAccess( 
            /* [in] */ BSTR bstrKey,
            /* [in] */ short sAccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveKey( 
            /* [in] */ BSTR bstrKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE BanObject( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UnbanObject( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Avatar( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring][id][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Avatar( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring][id][restricted] */ HRESULT STDMETHODCALLTYPE WriteToDisk( 
            /* [in] */ BSTR bstrFilename) = 0;
        
        virtual /* [helpstring][id][restricted] */ HRESULT STDMETHODCALLTYPE ReadFromDisk( 
            /* [in] */ BSTR bstrFilename) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAvatarProfileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAvatarProfile __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAvatarProfile __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IAvatarProfile __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IAvatarProfile __RPC_FAR * This);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RefCount )( 
            IAvatarProfile __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CLSID )( 
            IAvatarProfile __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IID )( 
            IAvatarProfile __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IAvatarProfile __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IAvatarProfile __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parent )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyName )( 
            IAvatarProfile __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyName )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IAvatarProfile __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Security )( 
            IAvatarProfile __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Security )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsOfType )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToAscii )( 
            IAvatarProfile __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            IAvatarProfile __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallToScript )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [helpstring][id][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IAvatarProfile __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Keys )( 
            IAvatarProfile __RPC_FAR * This,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppKeyList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValid )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbValid);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddKey )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstrKey,
            /* [in] */ VARIANT varKeyVal,
            /* [optional][in] */ VARIANT varStrGraphic,
            /* [optional][in] */ VARIANT varAccess,
            /* [optional][in] */ VARIANT varOwner);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_KeyValue )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarKeyVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_KeyValue )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstrKey,
            /* [in] */ VARIANT varKeyVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_KeyGraphic )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrGraphic);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_KeyGraphic )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstrKey,
            /* [in] */ BSTR bstrGraphic);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_KeyOwner )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppOwner);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_KeyAccess )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ short __RPC_FAR *sAccess);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_KeyAccess )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstrKey,
            /* [in] */ short sAccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveKey )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstrKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BanObject )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnbanObject )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Avatar )( 
            IAvatarProfile __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *pThing);
        
        /* [helpstring][id][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Avatar )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring][id][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteToDisk )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstrFilename);
        
        /* [helpstring][id][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadFromDisk )( 
            IAvatarProfile __RPC_FAR * This,
            /* [in] */ BSTR bstrFilename);
        
        END_INTERFACE
    } IAvatarProfileVtbl;

    interface IAvatarProfile
    {
        CONST_VTBL struct IAvatarProfileVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAvatarProfile_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAvatarProfile_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAvatarProfile_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAvatarProfile_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAvatarProfile_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAvatarProfile_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAvatarProfile_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAvatarProfile_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IAvatarProfile_get_RefCount(This,pdwRef)	\
    (This)->lpVtbl -> get_RefCount(This,pdwRef)

#define IAvatarProfile_get_CLSID(This,pclsid)	\
    (This)->lpVtbl -> get_CLSID(This,pclsid)

#define IAvatarProfile_get_IID(This,piid)	\
    (This)->lpVtbl -> get_IID(This,piid)

#define IAvatarProfile_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IAvatarProfile_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define IAvatarProfile_get_Parent(This,ppthing)	\
    (This)->lpVtbl -> get_Parent(This,ppthing)

#define IAvatarProfile_put_Parent(This,pthing)	\
    (This)->lpVtbl -> put_Parent(This,pthing)

#define IAvatarProfile_get_PropertyName(This,pbstr)	\
    (This)->lpVtbl -> get_PropertyName(This,pbstr)

#define IAvatarProfile_put_PropertyName(This,bstr)	\
    (This)->lpVtbl -> put_PropertyName(This,bstr)

#define IAvatarProfile_get_Type(This,pbstr)	\
    (This)->lpVtbl -> get_Type(This,pbstr)

#define IAvatarProfile_put_Type(This,bstr)	\
    (This)->lpVtbl -> put_Type(This,bstr)

#define IAvatarProfile_get_Security(This,ppSecurity)	\
    (This)->lpVtbl -> get_Security(This,ppSecurity)

#define IAvatarProfile_put_Security(This,pSecurity)	\
    (This)->lpVtbl -> put_Security(This,pSecurity)

#define IAvatarProfile_IsOfType(This,bstr,pbool)	\
    (This)->lpVtbl -> IsOfType(This,bstr,pbool)

#define IAvatarProfile_ToAscii(This,pbstr)	\
    (This)->lpVtbl -> ToAscii(This,pbstr)

#define IAvatarProfile_Dump(This)	\
    (This)->lpVtbl -> Dump(This)

#define IAvatarProfile_Clone(This,pobjectproperty)	\
    (This)->lpVtbl -> Clone(This,pobjectproperty)

#define IAvatarProfile_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define IAvatarProfile_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define IAvatarProfile_MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)


#define IAvatarProfile_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IAvatarProfile_get_Keys(This,ppKeyList)	\
    (This)->lpVtbl -> get_Keys(This,ppKeyList)

#define IAvatarProfile_IsValid(This,bstrKey,pbValid)	\
    (This)->lpVtbl -> IsValid(This,bstrKey,pbValid)

#define IAvatarProfile_AddKey(This,bstrKey,varKeyVal,varStrGraphic,varAccess,varOwner)	\
    (This)->lpVtbl -> AddKey(This,bstrKey,varKeyVal,varStrGraphic,varAccess,varOwner)

#define IAvatarProfile_get_KeyValue(This,bstrKey,pvarKeyVal)	\
    (This)->lpVtbl -> get_KeyValue(This,bstrKey,pvarKeyVal)

#define IAvatarProfile_put_KeyValue(This,bstrKey,varKeyVal)	\
    (This)->lpVtbl -> put_KeyValue(This,bstrKey,varKeyVal)

#define IAvatarProfile_get_KeyGraphic(This,bstrKey,pbstrGraphic)	\
    (This)->lpVtbl -> get_KeyGraphic(This,bstrKey,pbstrGraphic)

#define IAvatarProfile_put_KeyGraphic(This,bstrKey,bstrGraphic)	\
    (This)->lpVtbl -> put_KeyGraphic(This,bstrKey,bstrGraphic)

#define IAvatarProfile_get_KeyOwner(This,bstrKey,ppOwner)	\
    (This)->lpVtbl -> get_KeyOwner(This,bstrKey,ppOwner)

#define IAvatarProfile_get_KeyAccess(This,bstrKey,sAccess)	\
    (This)->lpVtbl -> get_KeyAccess(This,bstrKey,sAccess)

#define IAvatarProfile_put_KeyAccess(This,bstrKey,sAccess)	\
    (This)->lpVtbl -> put_KeyAccess(This,bstrKey,sAccess)

#define IAvatarProfile_RemoveKey(This,bstrKey)	\
    (This)->lpVtbl -> RemoveKey(This,bstrKey)

#define IAvatarProfile_BanObject(This,pThing)	\
    (This)->lpVtbl -> BanObject(This,pThing)

#define IAvatarProfile_UnbanObject(This,pThing)	\
    (This)->lpVtbl -> UnbanObject(This,pThing)

#define IAvatarProfile_get_Avatar(This,pThing)	\
    (This)->lpVtbl -> get_Avatar(This,pThing)

#define IAvatarProfile_put_Avatar(This,pThing)	\
    (This)->lpVtbl -> put_Avatar(This,pThing)

#define IAvatarProfile_WriteToDisk(This,bstrFilename)	\
    (This)->lpVtbl -> WriteToDisk(This,bstrFilename)

#define IAvatarProfile_ReadFromDisk(This,bstrFilename)	\
    (This)->lpVtbl -> ReadFromDisk(This,bstrFilename)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][restricted] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_Reset_Proxy( 
    IAvatarProfile __RPC_FAR * This);


void __RPC_STUB IAvatarProfile_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_get_Keys_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppKeyList);


void __RPC_STUB IAvatarProfile_get_Keys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_IsValid_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbValid);


void __RPC_STUB IAvatarProfile_IsValid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_AddKey_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [in] */ VARIANT varKeyVal,
    /* [optional][in] */ VARIANT varStrGraphic,
    /* [optional][in] */ VARIANT varAccess,
    /* [optional][in] */ VARIANT varOwner);


void __RPC_STUB IAvatarProfile_AddKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_get_KeyValue_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarKeyVal);


void __RPC_STUB IAvatarProfile_get_KeyValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_put_KeyValue_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [in] */ VARIANT varKeyVal);


void __RPC_STUB IAvatarProfile_put_KeyValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_get_KeyGraphic_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrGraphic);


void __RPC_STUB IAvatarProfile_get_KeyGraphic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_put_KeyGraphic_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [in] */ BSTR bstrGraphic);


void __RPC_STUB IAvatarProfile_put_KeyGraphic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_get_KeyOwner_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppOwner);


void __RPC_STUB IAvatarProfile_get_KeyOwner_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_get_KeyAccess_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [retval][out] */ short __RPC_FAR *sAccess);


void __RPC_STUB IAvatarProfile_get_KeyAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_put_KeyAccess_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [in] */ short sAccess);


void __RPC_STUB IAvatarProfile_put_KeyAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_RemoveKey_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey);


void __RPC_STUB IAvatarProfile_RemoveKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_BanObject_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB IAvatarProfile_BanObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_UnbanObject_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB IAvatarProfile_UnbanObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_get_Avatar_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *pThing);


void __RPC_STUB IAvatarProfile_get_Avatar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput][restricted] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_put_Avatar_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB IAvatarProfile_put_Avatar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][restricted] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_WriteToDisk_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrFilename);


void __RPC_STUB IAvatarProfile_WriteToDisk_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][restricted] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_ReadFromDisk_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrFilename);


void __RPC_STUB IAvatarProfile_ReadFromDisk_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAvatarProfile_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_avprof_0244 */
/* [local] */ 

typedef /* [helpstring][uuid] */ 
enum KeyAccessConstant
    {	kKeyPublic	= 0,
	kKeyFriend	= 1,
	kKeyPrivate	= 2
    }	KeyAccessConstant;

typedef /* [helpstring][uuid] */ 
enum ProfileHint
    {	profileAddKey	= 0,
	profileKeyValue	= 1,
	profileKeyGraphic	= 2,
	profileKeyOwner	= 3,
	profileKeyAccess	= 4,
	profileRemoveKey	= 5,
	profileBanObject	= 6,
	profileUnbanObject	= 7
    }	ProfileHint;



extern RPC_IF_HANDLE __MIDL_itf_avprof_0244_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_avprof_0244_v0_0_s_ifspec;

#ifndef __IAvatarProfileData_INTERFACE_DEFINED__
#define __IAvatarProfileData_INTERFACE_DEFINED__

/* interface IAvatarProfileData */
/* [unique][helpstring][dual][uuid][object] */ 

#define	DISPID_AVATARPROFILEDATA_VALUE	( 1 )

#define	DISPID_AVATARPROFILEDATA_GRAPHIC	( 2 )

#define	DISPID_AVATARPROFILEDATA_OWNER	( 3 )

#define	DISPID_AVATARPROFILEDATA_ACCESS	( 4 )


EXTERN_C const IID IID_IAvatarProfileData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D4E7472D-2953-11D1-AFB6-00C04FB915AA")
    IAvatarProfileData : public IObjectProperty
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvarValue) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            /* [in] */ VARIANT varValue) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Graphic( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrGraphic) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Graphic( 
            /* [in] */ BSTR bstrGraphic) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Owner( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppOwner) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Owner( 
            /* [in] */ IThing __RPC_FAR *pOwner) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Access( 
            /* [retval][out] */ short __RPC_FAR *sAccess) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Access( 
            /* [in] */ short sAccess) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAvatarProfileDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAvatarProfileData __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAvatarProfileData __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IAvatarProfileData __RPC_FAR * This);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RefCount )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CLSID )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IID )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parent )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyName )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyName )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Security )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Security )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsOfType )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToAscii )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            IAvatarProfileData __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallToScript )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Value )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarValue);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Value )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ VARIANT varValue);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Graphic )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrGraphic);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Graphic )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ BSTR bstrGraphic);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Owner )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppOwner);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Owner )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pOwner);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Access )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *sAccess);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Access )( 
            IAvatarProfileData __RPC_FAR * This,
            /* [in] */ short sAccess);
        
        END_INTERFACE
    } IAvatarProfileDataVtbl;

    interface IAvatarProfileData
    {
        CONST_VTBL struct IAvatarProfileDataVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAvatarProfileData_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAvatarProfileData_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAvatarProfileData_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAvatarProfileData_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAvatarProfileData_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAvatarProfileData_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAvatarProfileData_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAvatarProfileData_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IAvatarProfileData_get_RefCount(This,pdwRef)	\
    (This)->lpVtbl -> get_RefCount(This,pdwRef)

#define IAvatarProfileData_get_CLSID(This,pclsid)	\
    (This)->lpVtbl -> get_CLSID(This,pclsid)

#define IAvatarProfileData_get_IID(This,piid)	\
    (This)->lpVtbl -> get_IID(This,piid)

#define IAvatarProfileData_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IAvatarProfileData_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define IAvatarProfileData_get_Parent(This,ppthing)	\
    (This)->lpVtbl -> get_Parent(This,ppthing)

#define IAvatarProfileData_put_Parent(This,pthing)	\
    (This)->lpVtbl -> put_Parent(This,pthing)

#define IAvatarProfileData_get_PropertyName(This,pbstr)	\
    (This)->lpVtbl -> get_PropertyName(This,pbstr)

#define IAvatarProfileData_put_PropertyName(This,bstr)	\
    (This)->lpVtbl -> put_PropertyName(This,bstr)

#define IAvatarProfileData_get_Type(This,pbstr)	\
    (This)->lpVtbl -> get_Type(This,pbstr)

#define IAvatarProfileData_put_Type(This,bstr)	\
    (This)->lpVtbl -> put_Type(This,bstr)

#define IAvatarProfileData_get_Security(This,ppSecurity)	\
    (This)->lpVtbl -> get_Security(This,ppSecurity)

#define IAvatarProfileData_put_Security(This,pSecurity)	\
    (This)->lpVtbl -> put_Security(This,pSecurity)

#define IAvatarProfileData_IsOfType(This,bstr,pbool)	\
    (This)->lpVtbl -> IsOfType(This,bstr,pbool)

#define IAvatarProfileData_ToAscii(This,pbstr)	\
    (This)->lpVtbl -> ToAscii(This,pbstr)

#define IAvatarProfileData_Dump(This)	\
    (This)->lpVtbl -> Dump(This)

#define IAvatarProfileData_Clone(This,pobjectproperty)	\
    (This)->lpVtbl -> Clone(This,pobjectproperty)

#define IAvatarProfileData_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define IAvatarProfileData_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define IAvatarProfileData_MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)


#define IAvatarProfileData_get_Value(This,pvarValue)	\
    (This)->lpVtbl -> get_Value(This,pvarValue)

#define IAvatarProfileData_put_Value(This,varValue)	\
    (This)->lpVtbl -> put_Value(This,varValue)

#define IAvatarProfileData_get_Graphic(This,pbstrGraphic)	\
    (This)->lpVtbl -> get_Graphic(This,pbstrGraphic)

#define IAvatarProfileData_put_Graphic(This,bstrGraphic)	\
    (This)->lpVtbl -> put_Graphic(This,bstrGraphic)

#define IAvatarProfileData_get_Owner(This,ppOwner)	\
    (This)->lpVtbl -> get_Owner(This,ppOwner)

#define IAvatarProfileData_put_Owner(This,pOwner)	\
    (This)->lpVtbl -> put_Owner(This,pOwner)

#define IAvatarProfileData_get_Access(This,sAccess)	\
    (This)->lpVtbl -> get_Access(This,sAccess)

#define IAvatarProfileData_put_Access(This,sAccess)	\
    (This)->lpVtbl -> put_Access(This,sAccess)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfileData_get_Value_Proxy( 
    IAvatarProfileData __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarValue);


void __RPC_STUB IAvatarProfileData_get_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAvatarProfileData_put_Value_Proxy( 
    IAvatarProfileData __RPC_FAR * This,
    /* [in] */ VARIANT varValue);


void __RPC_STUB IAvatarProfileData_put_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfileData_get_Graphic_Proxy( 
    IAvatarProfileData __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrGraphic);


void __RPC_STUB IAvatarProfileData_get_Graphic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAvatarProfileData_put_Graphic_Proxy( 
    IAvatarProfileData __RPC_FAR * This,
    /* [in] */ BSTR bstrGraphic);


void __RPC_STUB IAvatarProfileData_put_Graphic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfileData_get_Owner_Proxy( 
    IAvatarProfileData __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppOwner);


void __RPC_STUB IAvatarProfileData_get_Owner_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAvatarProfileData_put_Owner_Proxy( 
    IAvatarProfileData __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pOwner);


void __RPC_STUB IAvatarProfileData_put_Owner_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfileData_get_Access_Proxy( 
    IAvatarProfileData __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *sAccess);


void __RPC_STUB IAvatarProfileData_get_Access_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAvatarProfileData_put_Access_Proxy( 
    IAvatarProfileData __RPC_FAR * This,
    /* [in] */ short sAccess);


void __RPC_STUB IAvatarProfileData_put_Access_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAvatarProfileData_INTERFACE_DEFINED__ */


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
