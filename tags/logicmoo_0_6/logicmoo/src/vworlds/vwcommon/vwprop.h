/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:27 2000
 */
/* Compiler settings for vwprop.idl:
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

#ifndef __vwprop_h__
#define __vwprop_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IPropertySecurity_FWD_DEFINED__
#define __IPropertySecurity_FWD_DEFINED__
typedef interface IPropertySecurity IPropertySecurity;
#endif 	/* __IPropertySecurity_FWD_DEFINED__ */


#ifndef __IObjectProperty_FWD_DEFINED__
#define __IObjectProperty_FWD_DEFINED__
typedef interface IObjectProperty IObjectProperty;
#endif 	/* __IObjectProperty_FWD_DEFINED__ */


#ifndef __IPropertyCollection_FWD_DEFINED__
#define __IPropertyCollection_FWD_DEFINED__
typedef interface IPropertyCollection IPropertyCollection;
#endif 	/* __IPropertyCollection_FWD_DEFINED__ */


#ifndef __IPropertyList_FWD_DEFINED__
#define __IPropertyList_FWD_DEFINED__
typedef interface IPropertyList IPropertyList;
#endif 	/* __IPropertyList_FWD_DEFINED__ */


#ifndef __IPropertyMap_FWD_DEFINED__
#define __IPropertyMap_FWD_DEFINED__
typedef interface IPropertyMap IPropertyMap;
#endif 	/* __IPropertyMap_FWD_DEFINED__ */


#ifndef __IThing_FWD_DEFINED__
#define __IThing_FWD_DEFINED__
typedef interface IThing IThing;
#endif 	/* __IThing_FWD_DEFINED__ */


#ifndef __IModule_FWD_DEFINED__
#define __IModule_FWD_DEFINED__
typedef interface IModule IModule;
#endif 	/* __IModule_FWD_DEFINED__ */


#ifndef __ICOMModule_FWD_DEFINED__
#define __ICOMModule_FWD_DEFINED__
typedef interface ICOMModule ICOMModule;
#endif 	/* __ICOMModule_FWD_DEFINED__ */


#ifndef __IScriptModule_FWD_DEFINED__
#define __IScriptModule_FWD_DEFINED__
typedef interface IScriptModule IScriptModule;
#endif 	/* __IScriptModule_FWD_DEFINED__ */


#ifndef __IMethod_FWD_DEFINED__
#define __IMethod_FWD_DEFINED__
typedef interface IMethod IMethod;
#endif 	/* __IMethod_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "vwbuffer.h"
#include "vwobject.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_vwprop_0000 */
/* [local] */ 





extern RPC_IF_HANDLE __MIDL_itf_vwprop_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vwprop_0000_v0_0_s_ifspec;

#ifndef __IPropertySecurity_INTERFACE_DEFINED__
#define __IPropertySecurity_INTERFACE_DEFINED__

/* interface IPropertySecurity */
/* [unique][helpstring][dual][uuid][object] */ 

#define	PSBIT_NONE	( 0 )

#define	PSBIT_COPYONGET	( 0x1 )

#define	PSBIT_VOLATILE	( 0x2 )

#define	PSBIT_LOCAL	( 0x4 )

#define	PSBIT_NOTLOGGED	( PSBIT_VOLATILE )

#define	PSBIT_NOTREMOTED	( PSBIT_LOCAL )

#define	PSBIT_BYVAL	( 0x8 )

#define	PSBIT_OWNERONLY	( 0x10 )

#define	PSBIT_NOTIFYONCHANGE	( 0x20 )

#define	PSBIT_NOTIFYONACCESS	( 0x40 )

#define	PSBIT_FRIENDLY	( 0x80 )

#define	PSBIT_HIDDEN	( 0x100 )

#define	PSBIT_CLIENTPROPERTY	( 0x200 )

#define	PSBIT_INTERNALLYADDED	( 0x400 )

#define	PSBIT_INTERNALLYSET	( 0x800 )

#define	PSBIT_CONSTANT	( 0x1000 )

#define	PSBIT_ENABLED	( 0x1000000 )

#define	PSMASK_ENABLED	( 0xff000000 )

#define	PSBIT_SYSTEMOVERRIDE	( 0x2000000 )

#define	PS_NONE	( 0 )

#define	PS_READ	( 0x1 )

#define	PS_WRITE	( 0x2 )

#define	PS_EXECUTE	( 0x4 )

#define	PS_ADD	( 0x8 )

#define	PS_OWNER	( 0x1 )

#define	PS_EXEMPLAR	( 0x2 )

#define	PS_ALL	( 0x4 )

#define	PS_OWNER_R	( PS_READ )

#define	PS_OWNER_RW	( PS_READ | PS_WRITE )

#define	PS_OWNER_RWA	( PS_READ | PS_WRITE | PS_ADD )

#define	PS_OWNER_E	( PS_EXECUTE )

#define	PS_EXEMPLAR_R	( PS_READ << 8 )

#define	PS_EXEMPLAR_RW	( PS_READ << 8 | PS_WRITE << 8 )

#define	PS_EXEMPLAR_RWA	( PS_READ << 8 | PS_WRITE << 8 | PS_ADD << 8 )

#define	PS_EXEMPLAR_E	( PS_EXECUTE << 8 )

#define	PS_ALL_R	( PS_READ << 16 )

#define	PS_ALL_RW	( PS_READ << 16 | PS_WRITE << 16 )

#define	PS_ALL_RWA	( PS_READ << 16 | PS_WRITE << 16 | PS_ADD << 16 )

#define	PS_ALL_E	( PS_EXECUTE << 16 )

#define	PS_ALLACCESSPROPERTY	( PS_OWNER_RW | PS_EXEMPLAR_RW | PS_ALL_RW )

#define	PS_NOACCESSPROPERTY	( PS_NONE )

#define	PS_ALLEXECUTEMETHOD	( PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_R | PS_OWNER_E | PS_EXEMPLAR_E | PS_ALL_E )

#define	PS_GLOBALSYSTEMCONSTANT	( PS_ALLACCESSPROPERTY )

#define	PSBIT_GLOBALSYSTEMCONSTANT	( PSBIT_COPYONGET | PSBIT_INTERNALLYADDED | PSBIT_CONSTANT )

#define	PS_GLOBALDEFAULTMETHOD	( PS_OWNER_RWA | PS_EXECUTE | PS_EXEMPLAR_RW | PS_EXECUTE << 8 | PS_ALL_R | PS_ALL_E )

#define	PSBIT_GLOBALDEFAULTMETHOD	( PSBIT_INTERNALLYADDED | PSBIT_INTERNALLYSET )

#define	PS_WORLDOWNERPROPERTY	( PS_ALLACCESSPROPERTY )

#define	PSBIT_WORLDOWNERPROPERTY	( PSBIT_COPYONGET | PSBIT_INTERNALLYADDED )

#define	PS_WORLDOWNERCOLLECTION	( PS_OWNER_RWA | PS_READ << 8 | PS_READ << 16 )

#define	PS_SYSTEMPROPERTY	( PS_OWNER_RW | PS_EXEMPLAR_RW | PS_ALL_R )

#define	PSBIT_SYSTEMPROPERTY	( PSBIT_COPYONGET | PSBIT_INTERNALLYADDED )

#define	PS_EVENTHANDLER	( PS_READ | PS_EXECUTE | PS_READ << 8 | PS_EXECUTE << 8 | PS_ALL_R )

#define	PSBIT_EVENTHANDLER	( PSBIT_INTERNALLYADDED )

#define	PSBIT_SYSTEMEVENTHANDLER	( PSBIT_INTERNALLYADDED | PSBIT_INTERNALLYSET | PSBIT_HIDDEN )

#define	PS_EXEMPLARDEFAULTCOLLECTION	( PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_R )

#define	PS_EXEMPLARDEFAULTPROPERTY	( PS_OWNER_RWA | PS_EXEMPLAR_RW | PS_ALL_R )

#define	PSBIT_EXEMPLARDEFAULTPROPERTY	( PSBIT_COPYONGET | PSBIT_INTERNALLYADDED )

#define	PS_EXEMPLARDEFAULTMETHOD	( PS_OWNER_RWA | PS_EXECUTE | PS_EXEMPLAR_RW | PS_EXECUTE << 8 | PS_ALL_R )

#define	PSBIT_EXEMPLARDEFAULTMETHOD	( PSBIT_INTERNALLYADDED )

#define	PS_SCRIPTDEFAULTCOLLECTION	( PS_EXEMPLARDEFAULTCOLLECTION )

#define	PS_SCRIPTDEFAULTPROPERTY	( PS_EXEMPLARDEFAULTPROPERTY )

#define	PSBIT_SCRIPTDEFAULTPROPERTY	( PSBIT_COPYONGET )

#define	PS_SCRIPTDEFAULTMETHOD	( PS_EXEMPLARDEFAULTMETHOD )

#define	PSBIT_SCRIPTDEFAULTMETHOD	( PSBIT_NONE )


EXTERN_C const IID IID_IPropertySecurity;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D9842-A417-11CF-9BF0-0080C7A56A8A")
    IPropertySecurity : public IDispatch
    {
    public:
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ IWorld __RPC_FAR *pWorld,
            /* [in] */ IThing __RPC_FAR *pExemplar,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Cleanup( void) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_World( 
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_World( 
            /* [in] */ IWorld __RPC_FAR *pworld) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Exemplar( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppExemplar) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Exemplar( 
            /* [in] */ IThing __RPC_FAR *pExemplar) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Instance( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppInstance) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Instance( 
            /* [in] */ IThing __RPC_FAR *pInstance) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Property( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Property( 
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Permissions( 
            /* [retval][out] */ long __RPC_FAR *ulFlags) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Permissions( 
            /* [in] */ long ulFlags) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Bits( 
            /* [retval][out] */ long __RPC_FAR *bits) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Bits( 
            /* [in] */ long bits) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_BitState( 
            /* [in] */ long bits,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSet) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_BitState( 
            /* [in] */ long bits,
            /* [in] */ VARIANT_BOOL bSet) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_vtType( 
            /* [retval][out] */ unsigned short __RPC_FAR *vartype) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_vtType( 
            /* [in] */ unsigned short vartype) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_PropertyIID( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrIID) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_PropertyIID( 
            /* [in] */ BSTR bstrIID) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_PropertyIIDExt( 
            /* [retval][out] */ IID __RPC_FAR *piid) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_PropertyIIDExt( 
            /* [in] */ REFIID riid) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_PropertyType( 
            /* [retval][out] */ BSTR __RPC_FAR *bstrType) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_PropertyType( 
            /* [in] */ BSTR bstrType) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Marshall( 
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE UnMarshall( 
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ChangeMode( 
            /* [in] */ BSTR bstrMode) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE QueryMode( 
            /* [in] */ BSTR bstrGroup,
            /* [in] */ BSTR bstrPermission,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bPermitted) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CheckSecurity( 
            /* [in] */ unsigned long ulFlags,
            /* [in] */ VARIANT_BOOL bQuiet,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbSuccess) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CopyPermission( 
            /* [in] */ IPropertySecurity __RPC_FAR *pFrom) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Enable( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPropertySecurityVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPropertySecurity __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPropertySecurity __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPropertySecurity __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pWorld,
            /* [in] */ IThing __RPC_FAR *pExemplar,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Cleanup )( 
            IPropertySecurity __RPC_FAR * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IPropertySecurity __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Exemplar )( 
            IPropertySecurity __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppExemplar);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Exemplar )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pExemplar);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Instance )( 
            IPropertySecurity __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppInstance);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Instance )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pInstance);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Property )( 
            IPropertySecurity __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Property )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ VARIANT var);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Permissions )( 
            IPropertySecurity __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *ulFlags);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Permissions )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ long ulFlags);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Bits )( 
            IPropertySecurity __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *bits);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Bits )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ long bits);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BitState )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ long bits,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSet);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BitState )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ long bits,
            /* [in] */ VARIANT_BOOL bSet);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_vtType )( 
            IPropertySecurity __RPC_FAR * This,
            /* [retval][out] */ unsigned short __RPC_FAR *vartype);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_vtType )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ unsigned short vartype);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyIID )( 
            IPropertySecurity __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrIID);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyIID )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ BSTR bstrIID);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyIIDExt )( 
            IPropertySecurity __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyIIDExt )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ REFIID riid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyType )( 
            IPropertySecurity __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *bstrType);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyType )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ BSTR bstrType);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ChangeMode )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ BSTR bstrMode);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryMode )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ BSTR bstrGroup,
            /* [in] */ BSTR bstrPermission,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bPermitted);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckSecurity )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ VARIANT_BOOL bQuiet,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbSuccess);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CopyPermission )( 
            IPropertySecurity __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pFrom);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Enable )( 
            IPropertySecurity __RPC_FAR * This);
        
        END_INTERFACE
    } IPropertySecurityVtbl;

    interface IPropertySecurity
    {
        CONST_VTBL struct IPropertySecurityVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPropertySecurity_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPropertySecurity_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPropertySecurity_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPropertySecurity_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPropertySecurity_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPropertySecurity_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPropertySecurity_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPropertySecurity_Initialize(This,pWorld,pExemplar,pSecurity)	\
    (This)->lpVtbl -> Initialize(This,pWorld,pExemplar,pSecurity)

#define IPropertySecurity_Cleanup(This)	\
    (This)->lpVtbl -> Cleanup(This)

#define IPropertySecurity_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IPropertySecurity_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define IPropertySecurity_get_Exemplar(This,ppExemplar)	\
    (This)->lpVtbl -> get_Exemplar(This,ppExemplar)

#define IPropertySecurity_put_Exemplar(This,pExemplar)	\
    (This)->lpVtbl -> put_Exemplar(This,pExemplar)

#define IPropertySecurity_get_Instance(This,ppInstance)	\
    (This)->lpVtbl -> get_Instance(This,ppInstance)

#define IPropertySecurity_put_Instance(This,pInstance)	\
    (This)->lpVtbl -> put_Instance(This,pInstance)

#define IPropertySecurity_get_Property(This,pvar)	\
    (This)->lpVtbl -> get_Property(This,pvar)

#define IPropertySecurity_put_Property(This,var)	\
    (This)->lpVtbl -> put_Property(This,var)

#define IPropertySecurity_get_Permissions(This,ulFlags)	\
    (This)->lpVtbl -> get_Permissions(This,ulFlags)

#define IPropertySecurity_put_Permissions(This,ulFlags)	\
    (This)->lpVtbl -> put_Permissions(This,ulFlags)

#define IPropertySecurity_get_Bits(This,bits)	\
    (This)->lpVtbl -> get_Bits(This,bits)

#define IPropertySecurity_put_Bits(This,bits)	\
    (This)->lpVtbl -> put_Bits(This,bits)

#define IPropertySecurity_get_BitState(This,bits,bSet)	\
    (This)->lpVtbl -> get_BitState(This,bits,bSet)

#define IPropertySecurity_put_BitState(This,bits,bSet)	\
    (This)->lpVtbl -> put_BitState(This,bits,bSet)

#define IPropertySecurity_get_vtType(This,vartype)	\
    (This)->lpVtbl -> get_vtType(This,vartype)

#define IPropertySecurity_put_vtType(This,vartype)	\
    (This)->lpVtbl -> put_vtType(This,vartype)

#define IPropertySecurity_get_PropertyIID(This,pbstrIID)	\
    (This)->lpVtbl -> get_PropertyIID(This,pbstrIID)

#define IPropertySecurity_put_PropertyIID(This,bstrIID)	\
    (This)->lpVtbl -> put_PropertyIID(This,bstrIID)

#define IPropertySecurity_get_PropertyIIDExt(This,piid)	\
    (This)->lpVtbl -> get_PropertyIIDExt(This,piid)

#define IPropertySecurity_put_PropertyIIDExt(This,riid)	\
    (This)->lpVtbl -> put_PropertyIIDExt(This,riid)

#define IPropertySecurity_get_PropertyType(This,bstrType)	\
    (This)->lpVtbl -> get_PropertyType(This,bstrType)

#define IPropertySecurity_put_PropertyType(This,bstrType)	\
    (This)->lpVtbl -> put_PropertyType(This,bstrType)

#define IPropertySecurity_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define IPropertySecurity_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define IPropertySecurity_ChangeMode(This,bstrMode)	\
    (This)->lpVtbl -> ChangeMode(This,bstrMode)

#define IPropertySecurity_QueryMode(This,bstrGroup,bstrPermission,bPermitted)	\
    (This)->lpVtbl -> QueryMode(This,bstrGroup,bstrPermission,bPermitted)

#define IPropertySecurity_CheckSecurity(This,ulFlags,bQuiet,pbSuccess)	\
    (This)->lpVtbl -> CheckSecurity(This,ulFlags,bQuiet,pbSuccess)

#define IPropertySecurity_CopyPermission(This,pFrom)	\
    (This)->lpVtbl -> CopyPermission(This,pFrom)

#define IPropertySecurity_Enable(This)	\
    (This)->lpVtbl -> Enable(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_Initialize_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ IWorld __RPC_FAR *pWorld,
    /* [in] */ IThing __RPC_FAR *pExemplar,
    /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);


void __RPC_STUB IPropertySecurity_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_Cleanup_Proxy( 
    IPropertySecurity __RPC_FAR * This);


void __RPC_STUB IPropertySecurity_Cleanup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_get_World_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);


void __RPC_STUB IPropertySecurity_get_World_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_put_World_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ IWorld __RPC_FAR *pworld);


void __RPC_STUB IPropertySecurity_put_World_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_get_Exemplar_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppExemplar);


void __RPC_STUB IPropertySecurity_get_Exemplar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_put_Exemplar_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pExemplar);


void __RPC_STUB IPropertySecurity_put_Exemplar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_get_Instance_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppInstance);


void __RPC_STUB IPropertySecurity_get_Instance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_put_Instance_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pInstance);


void __RPC_STUB IPropertySecurity_put_Instance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_get_Property_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IPropertySecurity_get_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_put_Property_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ VARIANT var);


void __RPC_STUB IPropertySecurity_put_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_get_Permissions_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *ulFlags);


void __RPC_STUB IPropertySecurity_get_Permissions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_put_Permissions_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ long ulFlags);


void __RPC_STUB IPropertySecurity_put_Permissions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_get_Bits_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *bits);


void __RPC_STUB IPropertySecurity_get_Bits_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_put_Bits_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ long bits);


void __RPC_STUB IPropertySecurity_put_Bits_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_get_BitState_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ long bits,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSet);


void __RPC_STUB IPropertySecurity_get_BitState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_put_BitState_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ long bits,
    /* [in] */ VARIANT_BOOL bSet);


void __RPC_STUB IPropertySecurity_put_BitState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_get_vtType_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [retval][out] */ unsigned short __RPC_FAR *vartype);


void __RPC_STUB IPropertySecurity_get_vtType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_put_vtType_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ unsigned short vartype);


void __RPC_STUB IPropertySecurity_put_vtType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_get_PropertyIID_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrIID);


void __RPC_STUB IPropertySecurity_get_PropertyIID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_put_PropertyIID_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ BSTR bstrIID);


void __RPC_STUB IPropertySecurity_put_PropertyIID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_get_PropertyIIDExt_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [retval][out] */ IID __RPC_FAR *piid);


void __RPC_STUB IPropertySecurity_get_PropertyIIDExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_put_PropertyIIDExt_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ REFIID riid);


void __RPC_STUB IPropertySecurity_put_PropertyIIDExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_get_PropertyType_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *bstrType);


void __RPC_STUB IPropertySecurity_get_PropertyType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_put_PropertyType_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ BSTR bstrType);


void __RPC_STUB IPropertySecurity_put_PropertyType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_Marshall_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ DWORD dwOptions,
    /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);


void __RPC_STUB IPropertySecurity_Marshall_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_UnMarshall_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);


void __RPC_STUB IPropertySecurity_UnMarshall_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_ChangeMode_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ BSTR bstrMode);


void __RPC_STUB IPropertySecurity_ChangeMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_QueryMode_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ BSTR bstrGroup,
    /* [in] */ BSTR bstrPermission,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bPermitted);


void __RPC_STUB IPropertySecurity_QueryMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_CheckSecurity_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ VARIANT_BOOL bQuiet,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbSuccess);


void __RPC_STUB IPropertySecurity_CheckSecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_CopyPermission_Proxy( 
    IPropertySecurity __RPC_FAR * This,
    /* [in] */ IPropertySecurity __RPC_FAR *pFrom);


void __RPC_STUB IPropertySecurity_CopyPermission_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertySecurity_Enable_Proxy( 
    IPropertySecurity __RPC_FAR * This);


void __RPC_STUB IPropertySecurity_Enable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPropertySecurity_INTERFACE_DEFINED__ */


#ifndef __IObjectProperty_INTERFACE_DEFINED__
#define __IObjectProperty_INTERFACE_DEFINED__

/* interface IObjectProperty */
/* [unique][helpstring][dual][uuid][object] */ 

#define	DISPID_PROPERTY_TYPE	( 100 )


EXTERN_C const IID IID_IObjectProperty;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D9840-A417-11CF-9BF0-0080C7A56A8A")
    IObjectProperty : public IDispatch
    {
    public:
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Terminate( void) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_RefCount( 
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_CLSID( 
            /* [retval][out] */ CLSID __RPC_FAR *pclsid) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_IID( 
            /* [retval][out] */ IID __RPC_FAR *piid) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_World( 
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_World( 
            /* [in] */ IWorld __RPC_FAR *pworld) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Parent( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Parent( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_PropertyName( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_PropertyName( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Type( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_Security( 
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Security( 
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsOfType( 
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ToAscii( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE Dump( void) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Marshall( 
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE UnMarshall( 
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE MarshallToScript( 
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IObjectPropertyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IObjectProperty __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IObjectProperty __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IObjectProperty __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IObjectProperty __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IObjectProperty __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IObjectProperty __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IObjectProperty __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IObjectProperty __RPC_FAR * This);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RefCount )( 
            IObjectProperty __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CLSID )( 
            IObjectProperty __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IID )( 
            IObjectProperty __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IObjectProperty __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            IObjectProperty __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IObjectProperty __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parent )( 
            IObjectProperty __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyName )( 
            IObjectProperty __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyName )( 
            IObjectProperty __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IObjectProperty __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IObjectProperty __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Security )( 
            IObjectProperty __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Security )( 
            IObjectProperty __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsOfType )( 
            IObjectProperty __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToAscii )( 
            IObjectProperty __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            IObjectProperty __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IObjectProperty __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            IObjectProperty __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            IObjectProperty __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallToScript )( 
            IObjectProperty __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        END_INTERFACE
    } IObjectPropertyVtbl;

    interface IObjectProperty
    {
        CONST_VTBL struct IObjectPropertyVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IObjectProperty_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IObjectProperty_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IObjectProperty_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IObjectProperty_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IObjectProperty_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IObjectProperty_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IObjectProperty_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IObjectProperty_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IObjectProperty_get_RefCount(This,pdwRef)	\
    (This)->lpVtbl -> get_RefCount(This,pdwRef)

#define IObjectProperty_get_CLSID(This,pclsid)	\
    (This)->lpVtbl -> get_CLSID(This,pclsid)

#define IObjectProperty_get_IID(This,piid)	\
    (This)->lpVtbl -> get_IID(This,piid)

#define IObjectProperty_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IObjectProperty_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define IObjectProperty_get_Parent(This,ppthing)	\
    (This)->lpVtbl -> get_Parent(This,ppthing)

#define IObjectProperty_put_Parent(This,pthing)	\
    (This)->lpVtbl -> put_Parent(This,pthing)

#define IObjectProperty_get_PropertyName(This,pbstr)	\
    (This)->lpVtbl -> get_PropertyName(This,pbstr)

#define IObjectProperty_put_PropertyName(This,bstr)	\
    (This)->lpVtbl -> put_PropertyName(This,bstr)

#define IObjectProperty_get_Type(This,pbstr)	\
    (This)->lpVtbl -> get_Type(This,pbstr)

#define IObjectProperty_put_Type(This,bstr)	\
    (This)->lpVtbl -> put_Type(This,bstr)

#define IObjectProperty_get_Security(This,ppSecurity)	\
    (This)->lpVtbl -> get_Security(This,ppSecurity)

#define IObjectProperty_put_Security(This,pSecurity)	\
    (This)->lpVtbl -> put_Security(This,pSecurity)

#define IObjectProperty_IsOfType(This,bstr,pbool)	\
    (This)->lpVtbl -> IsOfType(This,bstr,pbool)

#define IObjectProperty_ToAscii(This,pbstr)	\
    (This)->lpVtbl -> ToAscii(This,pbstr)

#define IObjectProperty_Dump(This)	\
    (This)->lpVtbl -> Dump(This)

#define IObjectProperty_Clone(This,pobjectproperty)	\
    (This)->lpVtbl -> Clone(This,pobjectproperty)

#define IObjectProperty_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define IObjectProperty_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define IObjectProperty_MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IObjectProperty_Terminate_Proxy( 
    IObjectProperty __RPC_FAR * This);


void __RPC_STUB IObjectProperty_Terminate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IObjectProperty_get_RefCount_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [retval][out] */ DWORD __RPC_FAR *pdwRef);


void __RPC_STUB IObjectProperty_get_RefCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IObjectProperty_get_CLSID_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [retval][out] */ CLSID __RPC_FAR *pclsid);


void __RPC_STUB IObjectProperty_get_CLSID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IObjectProperty_get_IID_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [retval][out] */ IID __RPC_FAR *piid);


void __RPC_STUB IObjectProperty_get_IID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IObjectProperty_get_World_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);


void __RPC_STUB IObjectProperty_get_World_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IObjectProperty_put_World_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [in] */ IWorld __RPC_FAR *pworld);


void __RPC_STUB IObjectProperty_put_World_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IObjectProperty_get_Parent_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IObjectProperty_get_Parent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IObjectProperty_put_Parent_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IObjectProperty_put_Parent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IObjectProperty_get_PropertyName_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IObjectProperty_get_PropertyName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IObjectProperty_put_PropertyName_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IObjectProperty_put_PropertyName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IObjectProperty_get_Type_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IObjectProperty_get_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE IObjectProperty_put_Type_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IObjectProperty_put_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IObjectProperty_get_Security_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);


void __RPC_STUB IObjectProperty_get_Security_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IObjectProperty_put_Security_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);


void __RPC_STUB IObjectProperty_put_Security_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IObjectProperty_IsOfType_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [in] */ BSTR bstr,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IObjectProperty_IsOfType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IObjectProperty_ToAscii_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IObjectProperty_ToAscii_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IObjectProperty_Dump_Proxy( 
    IObjectProperty __RPC_FAR * This);


void __RPC_STUB IObjectProperty_Dump_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IObjectProperty_Clone_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);


void __RPC_STUB IObjectProperty_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IObjectProperty_Marshall_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [in] */ DWORD dwOptions,
    /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);


void __RPC_STUB IObjectProperty_Marshall_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IObjectProperty_UnMarshall_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);


void __RPC_STUB IObjectProperty_UnMarshall_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IObjectProperty_MarshallToScript_Proxy( 
    IObjectProperty __RPC_FAR * This,
    /* [in] */ DWORD dwOptions,
    /* [in] */ BSTR bstrContext,
    /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
    /* [out] */ BSTR __RPC_FAR *pbstrOut);


void __RPC_STUB IObjectProperty_MarshallToScript_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IObjectProperty_INTERFACE_DEFINED__ */


#ifndef __IPropertyCollection_INTERFACE_DEFINED__
#define __IPropertyCollection_INTERFACE_DEFINED__

/* interface IPropertyCollection */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPropertyCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D9844-A417-11CF-9BF0-0080C7A56A8A")
    IPropertyCollection : public IObjectProperty
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_IsEmpty( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *retval) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE FirstItem( 
            /* [out] */ BSTR __RPC_FAR *name,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE NextItem( 
            /* [out] */ BSTR __RPC_FAR *name,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_PropertyAtIndex( 
            /* [in] */ long index,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [id][hidden][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPropertyCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPropertyCollection __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPropertyCollection __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPropertyCollection __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IPropertyCollection __RPC_FAR * This);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RefCount )( 
            IPropertyCollection __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CLSID )( 
            IPropertyCollection __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IID )( 
            IPropertyCollection __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IPropertyCollection __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IPropertyCollection __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parent )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyName )( 
            IPropertyCollection __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyName )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IPropertyCollection __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Security )( 
            IPropertyCollection __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Security )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsOfType )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToAscii )( 
            IPropertyCollection __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            IPropertyCollection __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallToScript )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEmpty )( 
            IPropertyCollection __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IPropertyCollection __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *retval);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FirstItem )( 
            IPropertyCollection __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NextItem )( 
            IPropertyCollection __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyAtIndex )( 
            IPropertyCollection __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [id][hidden][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get__NewEnum )( 
            IPropertyCollection __RPC_FAR * This,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        END_INTERFACE
    } IPropertyCollectionVtbl;

    interface IPropertyCollection
    {
        CONST_VTBL struct IPropertyCollectionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPropertyCollection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPropertyCollection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPropertyCollection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPropertyCollection_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPropertyCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPropertyCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPropertyCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPropertyCollection_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IPropertyCollection_get_RefCount(This,pdwRef)	\
    (This)->lpVtbl -> get_RefCount(This,pdwRef)

#define IPropertyCollection_get_CLSID(This,pclsid)	\
    (This)->lpVtbl -> get_CLSID(This,pclsid)

#define IPropertyCollection_get_IID(This,piid)	\
    (This)->lpVtbl -> get_IID(This,piid)

#define IPropertyCollection_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IPropertyCollection_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define IPropertyCollection_get_Parent(This,ppthing)	\
    (This)->lpVtbl -> get_Parent(This,ppthing)

#define IPropertyCollection_put_Parent(This,pthing)	\
    (This)->lpVtbl -> put_Parent(This,pthing)

#define IPropertyCollection_get_PropertyName(This,pbstr)	\
    (This)->lpVtbl -> get_PropertyName(This,pbstr)

#define IPropertyCollection_put_PropertyName(This,bstr)	\
    (This)->lpVtbl -> put_PropertyName(This,bstr)

#define IPropertyCollection_get_Type(This,pbstr)	\
    (This)->lpVtbl -> get_Type(This,pbstr)

#define IPropertyCollection_put_Type(This,bstr)	\
    (This)->lpVtbl -> put_Type(This,bstr)

#define IPropertyCollection_get_Security(This,ppSecurity)	\
    (This)->lpVtbl -> get_Security(This,ppSecurity)

#define IPropertyCollection_put_Security(This,pSecurity)	\
    (This)->lpVtbl -> put_Security(This,pSecurity)

#define IPropertyCollection_IsOfType(This,bstr,pbool)	\
    (This)->lpVtbl -> IsOfType(This,bstr,pbool)

#define IPropertyCollection_ToAscii(This,pbstr)	\
    (This)->lpVtbl -> ToAscii(This,pbstr)

#define IPropertyCollection_Dump(This)	\
    (This)->lpVtbl -> Dump(This)

#define IPropertyCollection_Clone(This,pobjectproperty)	\
    (This)->lpVtbl -> Clone(This,pobjectproperty)

#define IPropertyCollection_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define IPropertyCollection_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define IPropertyCollection_MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)


#define IPropertyCollection_get_IsEmpty(This,pbool)	\
    (This)->lpVtbl -> get_IsEmpty(This,pbool)

#define IPropertyCollection_get_Count(This,retval)	\
    (This)->lpVtbl -> get_Count(This,retval)

#define IPropertyCollection_FirstItem(This,name,pvar,pbool)	\
    (This)->lpVtbl -> FirstItem(This,name,pvar,pbool)

#define IPropertyCollection_NextItem(This,name,pvar,pbool)	\
    (This)->lpVtbl -> NextItem(This,name,pvar,pbool)

#define IPropertyCollection_get_PropertyAtIndex(This,index,pvar)	\
    (This)->lpVtbl -> get_PropertyAtIndex(This,index,pvar)

#define IPropertyCollection_get__NewEnum(This,ppunk)	\
    (This)->lpVtbl -> get__NewEnum(This,ppunk)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertyCollection_get_IsEmpty_Proxy( 
    IPropertyCollection __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IPropertyCollection_get_IsEmpty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertyCollection_get_Count_Proxy( 
    IPropertyCollection __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *retval);


void __RPC_STUB IPropertyCollection_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyCollection_FirstItem_Proxy( 
    IPropertyCollection __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *name,
    /* [out] */ VARIANT __RPC_FAR *pvar,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IPropertyCollection_FirstItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyCollection_NextItem_Proxy( 
    IPropertyCollection __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *name,
    /* [out] */ VARIANT __RPC_FAR *pvar,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IPropertyCollection_NextItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyCollection_get_PropertyAtIndex_Proxy( 
    IPropertyCollection __RPC_FAR * This,
    /* [in] */ long index,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IPropertyCollection_get_PropertyAtIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][hidden][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertyCollection_get__NewEnum_Proxy( 
    IPropertyCollection __RPC_FAR * This,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);


void __RPC_STUB IPropertyCollection_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPropertyCollection_INTERFACE_DEFINED__ */


#ifndef __IPropertyList_INTERFACE_DEFINED__
#define __IPropertyList_INTERFACE_DEFINED__

/* interface IPropertyList */
/* [unique][helpstring][dual][uuid][object] */ 

#define	DISPID_PROPERTYLIST_ADD	( 101 )

#define	DISPID_PROPERTYLIST_PROPERTY	( 102 )

#define	DISPID_PROPERTYLIST_REMOVE	( 103 )

#define	DISPID_PROPERTYLIST_REMOVEAT	( 104 )

#define	DISPID_PROPERTYLIST_REMOVEALL	( 105 )

#define	DISPID_PROPERTYLIST_INSERTAT	( 106 )


EXTERN_C const IID IID_IPropertyList;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D9846-A417-11CF-9BF0-0080C7A56A8A")
    IPropertyList : public IPropertyCollection
    {
    public:
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE InsertAt( 
            /* [in] */ long index,
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Property( 
            /* [in] */ long index,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Property( 
            /* [in] */ long index,
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_PropertySecurity( 
            /* [in] */ long index,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE RemoveAt( 
            /* [in] */ long index) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE RemoveAll( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Find( 
            /* [in] */ VARIANT var,
            /* [retval][out] */ long __RPC_FAR *plval) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddString( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddObjectProperty( 
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddObjectPropertyExt( 
            /* [in] */ REFIID iid,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddThing( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddBOOL( 
            /* [in] */ VARIANT_BOOL boolVal) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddLong( 
            /* [in] */ long lval) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddShort( 
            /* [in] */ short sval) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddDispatch( 
            /* [in] */ IDispatch __RPC_FAR *pdisp) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_String( 
            /* [in] */ long index,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_String( 
            /* [in] */ long index,
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ObjectProperty( 
            /* [in] */ long index,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_ObjectProperty( 
            /* [in] */ long index,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ObjectPropertyExt( 
            /* [in] */ long index,
            /* [in] */ REFIID iid,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Thing( 
            /* [in] */ long index,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Thing( 
            /* [in] */ long index,
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_BOOL( 
            /* [in] */ long index,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_BOOL( 
            /* [in] */ long index,
            /* [in] */ VARIANT_BOOL boolVal) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Long( 
            /* [in] */ long index,
            /* [retval][out] */ long __RPC_FAR *plval) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Long( 
            /* [in] */ long index,
            /* [in] */ long lval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Dispatch( 
            /* [in] */ long index,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdisp) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Dispatch( 
            /* [in] */ long index,
            /* [in] */ IDispatch __RPC_FAR *pdisp) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE InsertStringAt( 
            /* [in] */ long index,
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE InsertObjectPropertyAt( 
            /* [in] */ long index,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE InsertThingAt( 
            /* [in] */ long index,
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE InsertBOOLAt( 
            /* [in] */ long index,
            /* [in] */ VARIANT_BOOL boolVal) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE InsertLongAt( 
            /* [in] */ long index,
            /* [in] */ long lval) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE InsertShortAt( 
            /* [in] */ long index,
            /* [in] */ short sval) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE InsertDispatchAt( 
            /* [in] */ long index,
            /* [in] */ IDispatch __RPC_FAR *pdisp) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE RemoveString( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE RemoveObjectProperty( 
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE RemoveThing( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE RemoveBOOL( 
            /* [in] */ VARIANT_BOOL boolVal) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE RemoveLong( 
            /* [in] */ long lval) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE RemoveShort( 
            /* [in] */ short sval) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE RemoveDispatch( 
            /* [in] */ IDispatch __RPC_FAR *pdisp) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CopyTo( 
            /* [in] */ IPropertyList __RPC_FAR *ppropertylist) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPropertyListVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPropertyList __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPropertyList __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPropertyList __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IPropertyList __RPC_FAR * This);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RefCount )( 
            IPropertyList __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CLSID )( 
            IPropertyList __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IID )( 
            IPropertyList __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IPropertyList __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IPropertyList __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parent )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyName )( 
            IPropertyList __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyName )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IPropertyList __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Security )( 
            IPropertyList __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Security )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsOfType )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToAscii )( 
            IPropertyList __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            IPropertyList __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallToScript )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEmpty )( 
            IPropertyList __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IPropertyList __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *retval);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FirstItem )( 
            IPropertyList __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NextItem )( 
            IPropertyList __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyAtIndex )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [id][hidden][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get__NewEnum )( 
            IPropertyList __RPC_FAR * This,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ VARIANT var);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertAt )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ VARIANT var);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Property )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Property )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ VARIANT var);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertySecurity )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Remove )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ VARIANT var);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveAt )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveAll )( 
            IPropertyList __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Find )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ VARIANT var,
            /* [retval][out] */ long __RPC_FAR *plval);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddString )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddObjectProperty )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddObjectPropertyExt )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ REFIID iid,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddThing )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddBOOL )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL boolVal);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddLong )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long lval);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddShort )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ short sval);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddDispatch )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pdisp);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_String )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_String )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ObjectProperty )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ObjectProperty )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ObjectPropertyExt )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ REFIID iid,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Thing )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Thing )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BOOL )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BOOL )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ VARIANT_BOOL boolVal);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Long )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ long __RPC_FAR *plval);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Long )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ long lval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dispatch )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdisp);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Dispatch )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ IDispatch __RPC_FAR *pdisp);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertStringAt )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertObjectPropertyAt )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertThingAt )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertBOOLAt )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ VARIANT_BOOL boolVal);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertLongAt )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ long lval);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertShortAt )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ short sval);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertDispatchAt )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ IDispatch __RPC_FAR *pdisp);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveString )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveObjectProperty )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveThing )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveBOOL )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL boolVal);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveLong )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ long lval);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveShort )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ short sval);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveDispatch )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pdisp);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CopyTo )( 
            IPropertyList __RPC_FAR * This,
            /* [in] */ IPropertyList __RPC_FAR *ppropertylist);
        
        END_INTERFACE
    } IPropertyListVtbl;

    interface IPropertyList
    {
        CONST_VTBL struct IPropertyListVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPropertyList_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPropertyList_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPropertyList_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPropertyList_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPropertyList_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPropertyList_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPropertyList_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPropertyList_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IPropertyList_get_RefCount(This,pdwRef)	\
    (This)->lpVtbl -> get_RefCount(This,pdwRef)

#define IPropertyList_get_CLSID(This,pclsid)	\
    (This)->lpVtbl -> get_CLSID(This,pclsid)

#define IPropertyList_get_IID(This,piid)	\
    (This)->lpVtbl -> get_IID(This,piid)

#define IPropertyList_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IPropertyList_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define IPropertyList_get_Parent(This,ppthing)	\
    (This)->lpVtbl -> get_Parent(This,ppthing)

#define IPropertyList_put_Parent(This,pthing)	\
    (This)->lpVtbl -> put_Parent(This,pthing)

#define IPropertyList_get_PropertyName(This,pbstr)	\
    (This)->lpVtbl -> get_PropertyName(This,pbstr)

#define IPropertyList_put_PropertyName(This,bstr)	\
    (This)->lpVtbl -> put_PropertyName(This,bstr)

#define IPropertyList_get_Type(This,pbstr)	\
    (This)->lpVtbl -> get_Type(This,pbstr)

#define IPropertyList_put_Type(This,bstr)	\
    (This)->lpVtbl -> put_Type(This,bstr)

#define IPropertyList_get_Security(This,ppSecurity)	\
    (This)->lpVtbl -> get_Security(This,ppSecurity)

#define IPropertyList_put_Security(This,pSecurity)	\
    (This)->lpVtbl -> put_Security(This,pSecurity)

#define IPropertyList_IsOfType(This,bstr,pbool)	\
    (This)->lpVtbl -> IsOfType(This,bstr,pbool)

#define IPropertyList_ToAscii(This,pbstr)	\
    (This)->lpVtbl -> ToAscii(This,pbstr)

#define IPropertyList_Dump(This)	\
    (This)->lpVtbl -> Dump(This)

#define IPropertyList_Clone(This,pobjectproperty)	\
    (This)->lpVtbl -> Clone(This,pobjectproperty)

#define IPropertyList_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define IPropertyList_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define IPropertyList_MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)


#define IPropertyList_get_IsEmpty(This,pbool)	\
    (This)->lpVtbl -> get_IsEmpty(This,pbool)

#define IPropertyList_get_Count(This,retval)	\
    (This)->lpVtbl -> get_Count(This,retval)

#define IPropertyList_FirstItem(This,name,pvar,pbool)	\
    (This)->lpVtbl -> FirstItem(This,name,pvar,pbool)

#define IPropertyList_NextItem(This,name,pvar,pbool)	\
    (This)->lpVtbl -> NextItem(This,name,pvar,pbool)

#define IPropertyList_get_PropertyAtIndex(This,index,pvar)	\
    (This)->lpVtbl -> get_PropertyAtIndex(This,index,pvar)

#define IPropertyList_get__NewEnum(This,ppunk)	\
    (This)->lpVtbl -> get__NewEnum(This,ppunk)


#define IPropertyList_Add(This,var)	\
    (This)->lpVtbl -> Add(This,var)

#define IPropertyList_InsertAt(This,index,var)	\
    (This)->lpVtbl -> InsertAt(This,index,var)

#define IPropertyList_get_Property(This,index,pvar)	\
    (This)->lpVtbl -> get_Property(This,index,pvar)

#define IPropertyList_put_Property(This,index,var)	\
    (This)->lpVtbl -> put_Property(This,index,var)

#define IPropertyList_get_PropertySecurity(This,index,ppSecurity)	\
    (This)->lpVtbl -> get_PropertySecurity(This,index,ppSecurity)

#define IPropertyList_Remove(This,var)	\
    (This)->lpVtbl -> Remove(This,var)

#define IPropertyList_RemoveAt(This,index)	\
    (This)->lpVtbl -> RemoveAt(This,index)

#define IPropertyList_RemoveAll(This)	\
    (This)->lpVtbl -> RemoveAll(This)

#define IPropertyList_Find(This,var,plval)	\
    (This)->lpVtbl -> Find(This,var,plval)

#define IPropertyList_AddString(This,bstr)	\
    (This)->lpVtbl -> AddString(This,bstr)

#define IPropertyList_AddObjectProperty(This,pobjectproperty)	\
    (This)->lpVtbl -> AddObjectProperty(This,pobjectproperty)

#define IPropertyList_AddObjectPropertyExt(This,iid,pobjectproperty)	\
    (This)->lpVtbl -> AddObjectPropertyExt(This,iid,pobjectproperty)

#define IPropertyList_AddThing(This,pthing)	\
    (This)->lpVtbl -> AddThing(This,pthing)

#define IPropertyList_AddBOOL(This,boolVal)	\
    (This)->lpVtbl -> AddBOOL(This,boolVal)

#define IPropertyList_AddLong(This,lval)	\
    (This)->lpVtbl -> AddLong(This,lval)

#define IPropertyList_AddShort(This,sval)	\
    (This)->lpVtbl -> AddShort(This,sval)

#define IPropertyList_AddDispatch(This,pdisp)	\
    (This)->lpVtbl -> AddDispatch(This,pdisp)

#define IPropertyList_get_String(This,index,pbstr)	\
    (This)->lpVtbl -> get_String(This,index,pbstr)

#define IPropertyList_put_String(This,index,bstr)	\
    (This)->lpVtbl -> put_String(This,index,bstr)

#define IPropertyList_get_ObjectProperty(This,index,ppobjectproperty)	\
    (This)->lpVtbl -> get_ObjectProperty(This,index,ppobjectproperty)

#define IPropertyList_put_ObjectProperty(This,index,pobjectproperty)	\
    (This)->lpVtbl -> put_ObjectProperty(This,index,pobjectproperty)

#define IPropertyList_get_ObjectPropertyExt(This,index,iid,ppobjectproperty)	\
    (This)->lpVtbl -> get_ObjectPropertyExt(This,index,iid,ppobjectproperty)

#define IPropertyList_get_Thing(This,index,ppthing)	\
    (This)->lpVtbl -> get_Thing(This,index,ppthing)

#define IPropertyList_put_Thing(This,index,pthing)	\
    (This)->lpVtbl -> put_Thing(This,index,pthing)

#define IPropertyList_get_BOOL(This,index,pboolVal)	\
    (This)->lpVtbl -> get_BOOL(This,index,pboolVal)

#define IPropertyList_put_BOOL(This,index,boolVal)	\
    (This)->lpVtbl -> put_BOOL(This,index,boolVal)

#define IPropertyList_get_Long(This,index,plval)	\
    (This)->lpVtbl -> get_Long(This,index,plval)

#define IPropertyList_put_Long(This,index,lval)	\
    (This)->lpVtbl -> put_Long(This,index,lval)

#define IPropertyList_get_Dispatch(This,index,ppdisp)	\
    (This)->lpVtbl -> get_Dispatch(This,index,ppdisp)

#define IPropertyList_put_Dispatch(This,index,pdisp)	\
    (This)->lpVtbl -> put_Dispatch(This,index,pdisp)

#define IPropertyList_InsertStringAt(This,index,bstr)	\
    (This)->lpVtbl -> InsertStringAt(This,index,bstr)

#define IPropertyList_InsertObjectPropertyAt(This,index,pobjectproperty)	\
    (This)->lpVtbl -> InsertObjectPropertyAt(This,index,pobjectproperty)

#define IPropertyList_InsertThingAt(This,index,pthing)	\
    (This)->lpVtbl -> InsertThingAt(This,index,pthing)

#define IPropertyList_InsertBOOLAt(This,index,boolVal)	\
    (This)->lpVtbl -> InsertBOOLAt(This,index,boolVal)

#define IPropertyList_InsertLongAt(This,index,lval)	\
    (This)->lpVtbl -> InsertLongAt(This,index,lval)

#define IPropertyList_InsertShortAt(This,index,sval)	\
    (This)->lpVtbl -> InsertShortAt(This,index,sval)

#define IPropertyList_InsertDispatchAt(This,index,pdisp)	\
    (This)->lpVtbl -> InsertDispatchAt(This,index,pdisp)

#define IPropertyList_RemoveString(This,bstr)	\
    (This)->lpVtbl -> RemoveString(This,bstr)

#define IPropertyList_RemoveObjectProperty(This,pobjectproperty)	\
    (This)->lpVtbl -> RemoveObjectProperty(This,pobjectproperty)

#define IPropertyList_RemoveThing(This,pthing)	\
    (This)->lpVtbl -> RemoveThing(This,pthing)

#define IPropertyList_RemoveBOOL(This,boolVal)	\
    (This)->lpVtbl -> RemoveBOOL(This,boolVal)

#define IPropertyList_RemoveLong(This,lval)	\
    (This)->lpVtbl -> RemoveLong(This,lval)

#define IPropertyList_RemoveShort(This,sval)	\
    (This)->lpVtbl -> RemoveShort(This,sval)

#define IPropertyList_RemoveDispatch(This,pdisp)	\
    (This)->lpVtbl -> RemoveDispatch(This,pdisp)

#define IPropertyList_CopyTo(This,ppropertylist)	\
    (This)->lpVtbl -> CopyTo(This,ppropertylist)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IPropertyList_Add_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ VARIANT var);


void __RPC_STUB IPropertyList_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IPropertyList_InsertAt_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ VARIANT var);


void __RPC_STUB IPropertyList_InsertAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertyList_get_Property_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IPropertyList_get_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE IPropertyList_put_Property_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ VARIANT var);


void __RPC_STUB IPropertyList_put_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IPropertyList_get_PropertySecurity_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);


void __RPC_STUB IPropertyList_get_PropertySecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IPropertyList_Remove_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ VARIANT var);


void __RPC_STUB IPropertyList_Remove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IPropertyList_RemoveAt_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index);


void __RPC_STUB IPropertyList_RemoveAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IPropertyList_RemoveAll_Proxy( 
    IPropertyList __RPC_FAR * This);


void __RPC_STUB IPropertyList_RemoveAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IPropertyList_Find_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ VARIANT var,
    /* [retval][out] */ long __RPC_FAR *plval);


void __RPC_STUB IPropertyList_Find_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_AddString_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IPropertyList_AddString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_AddObjectProperty_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);


void __RPC_STUB IPropertyList_AddObjectProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_AddObjectPropertyExt_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ REFIID iid,
    /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);


void __RPC_STUB IPropertyList_AddObjectPropertyExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_AddThing_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IPropertyList_AddThing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_AddBOOL_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL boolVal);


void __RPC_STUB IPropertyList_AddBOOL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_AddLong_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long lval);


void __RPC_STUB IPropertyList_AddLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_AddShort_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ short sval);


void __RPC_STUB IPropertyList_AddShort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_AddDispatch_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pdisp);


void __RPC_STUB IPropertyList_AddDispatch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_get_String_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IPropertyList_get_String_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_put_String_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ BSTR bstr);


void __RPC_STUB IPropertyList_put_String_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_get_ObjectProperty_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty);


void __RPC_STUB IPropertyList_get_ObjectProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_put_ObjectProperty_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);


void __RPC_STUB IPropertyList_put_ObjectProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_get_ObjectPropertyExt_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ REFIID iid,
    /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty);


void __RPC_STUB IPropertyList_get_ObjectPropertyExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_get_Thing_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IPropertyList_get_Thing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_put_Thing_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IPropertyList_put_Thing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_get_BOOL_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IPropertyList_get_BOOL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_put_BOOL_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ VARIANT_BOOL boolVal);


void __RPC_STUB IPropertyList_put_BOOL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_get_Long_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [retval][out] */ long __RPC_FAR *plval);


void __RPC_STUB IPropertyList_get_Long_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_put_Long_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ long lval);


void __RPC_STUB IPropertyList_put_Long_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_get_Dispatch_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdisp);


void __RPC_STUB IPropertyList_get_Dispatch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_put_Dispatch_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ IDispatch __RPC_FAR *pdisp);


void __RPC_STUB IPropertyList_put_Dispatch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_InsertStringAt_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ BSTR bstr);


void __RPC_STUB IPropertyList_InsertStringAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_InsertObjectPropertyAt_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);


void __RPC_STUB IPropertyList_InsertObjectPropertyAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_InsertThingAt_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IPropertyList_InsertThingAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_InsertBOOLAt_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ VARIANT_BOOL boolVal);


void __RPC_STUB IPropertyList_InsertBOOLAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_InsertLongAt_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ long lval);


void __RPC_STUB IPropertyList_InsertLongAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_InsertShortAt_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ short sval);


void __RPC_STUB IPropertyList_InsertShortAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_InsertDispatchAt_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ IDispatch __RPC_FAR *pdisp);


void __RPC_STUB IPropertyList_InsertDispatchAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_RemoveString_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IPropertyList_RemoveString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_RemoveObjectProperty_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);


void __RPC_STUB IPropertyList_RemoveObjectProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_RemoveThing_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IPropertyList_RemoveThing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_RemoveBOOL_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL boolVal);


void __RPC_STUB IPropertyList_RemoveBOOL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_RemoveLong_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ long lval);


void __RPC_STUB IPropertyList_RemoveLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_RemoveShort_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ short sval);


void __RPC_STUB IPropertyList_RemoveShort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyList_RemoveDispatch_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pdisp);


void __RPC_STUB IPropertyList_RemoveDispatch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IPropertyList_CopyTo_Proxy( 
    IPropertyList __RPC_FAR * This,
    /* [in] */ IPropertyList __RPC_FAR *ppropertylist);


void __RPC_STUB IPropertyList_CopyTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPropertyList_INTERFACE_DEFINED__ */


#ifndef __IPropertyMap_INTERFACE_DEFINED__
#define __IPropertyMap_INTERFACE_DEFINED__

/* interface IPropertyMap */
/* [unique][helpstring][dual][uuid][object] */ 

#define	DISPID_PROPERTYMAP_PROPERTY	( 101 )

#define	DISPID_PROPERTYMAP_REMOVE	( 102 )

#define	DISPID_PROPERTYMAP_REMOVEALL	( 103 )


EXTERN_C const IID IID_IPropertyMap;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D9848-A417-11CF-9BF0-0080C7A56A8A")
    IPropertyMap : public IPropertyCollection
    {
    public:
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Property( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Property( 
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_PropertyExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ UINT nHashOrig,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_PropertyInt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT var,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_PropertySecurity( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_PropertySecurityExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ UINT nHashOrig,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE RemoveInt( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE RemoveAll( void) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_String( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_String( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ObjectProperty( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_ObjectProperty( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ObjectPropertyExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ REFIID iid,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Thing( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Thing( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_BOOL( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_BOOL( 
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT_BOOL boolVal) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Long( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ long __RPC_FAR *plval) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Long( 
            /* [in] */ BSTR bstrName,
            /* [in] */ long lval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Short( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ short __RPC_FAR *psval) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Short( 
            /* [in] */ BSTR bstrName,
            /* [in] */ short sval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Dispatch( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdisp) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Dispatch( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IDispatch __RPC_FAR *pdisp) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsValid( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CopyTo( 
            /* [in] */ IPropertyMap __RPC_FAR *ppropertymap) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPropertyMapVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPropertyMap __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPropertyMap __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPropertyMap __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IPropertyMap __RPC_FAR * This);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RefCount )( 
            IPropertyMap __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CLSID )( 
            IPropertyMap __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IID )( 
            IPropertyMap __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IPropertyMap __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IPropertyMap __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parent )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyName )( 
            IPropertyMap __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyName )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IPropertyMap __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Security )( 
            IPropertyMap __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Security )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsOfType )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToAscii )( 
            IPropertyMap __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            IPropertyMap __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallToScript )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEmpty )( 
            IPropertyMap __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IPropertyMap __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *retval);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FirstItem )( 
            IPropertyMap __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NextItem )( 
            IPropertyMap __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyAtIndex )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [id][hidden][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get__NewEnum )( 
            IPropertyMap __RPC_FAR * This,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Property )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Property )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT var);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyExt )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ UINT nHashOrig,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyInt )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT var,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertySecurity )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertySecurityExt )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ UINT nHashOrig,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Remove )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveInt )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveAll )( 
            IPropertyMap __RPC_FAR * This);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_String )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_String )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ObjectProperty )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ObjectProperty )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ObjectPropertyExt )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ REFIID iid,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Thing )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Thing )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BOOL )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BOOL )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT_BOOL boolVal);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Long )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ long __RPC_FAR *plval);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Long )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ long lval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Short )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ short __RPC_FAR *psval);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Short )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ short sval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dispatch )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdisp);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Dispatch )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IDispatch __RPC_FAR *pdisp);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValid )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CopyTo )( 
            IPropertyMap __RPC_FAR * This,
            /* [in] */ IPropertyMap __RPC_FAR *ppropertymap);
        
        END_INTERFACE
    } IPropertyMapVtbl;

    interface IPropertyMap
    {
        CONST_VTBL struct IPropertyMapVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPropertyMap_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPropertyMap_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPropertyMap_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPropertyMap_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPropertyMap_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPropertyMap_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPropertyMap_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPropertyMap_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IPropertyMap_get_RefCount(This,pdwRef)	\
    (This)->lpVtbl -> get_RefCount(This,pdwRef)

#define IPropertyMap_get_CLSID(This,pclsid)	\
    (This)->lpVtbl -> get_CLSID(This,pclsid)

#define IPropertyMap_get_IID(This,piid)	\
    (This)->lpVtbl -> get_IID(This,piid)

#define IPropertyMap_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IPropertyMap_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define IPropertyMap_get_Parent(This,ppthing)	\
    (This)->lpVtbl -> get_Parent(This,ppthing)

#define IPropertyMap_put_Parent(This,pthing)	\
    (This)->lpVtbl -> put_Parent(This,pthing)

#define IPropertyMap_get_PropertyName(This,pbstr)	\
    (This)->lpVtbl -> get_PropertyName(This,pbstr)

#define IPropertyMap_put_PropertyName(This,bstr)	\
    (This)->lpVtbl -> put_PropertyName(This,bstr)

#define IPropertyMap_get_Type(This,pbstr)	\
    (This)->lpVtbl -> get_Type(This,pbstr)

#define IPropertyMap_put_Type(This,bstr)	\
    (This)->lpVtbl -> put_Type(This,bstr)

#define IPropertyMap_get_Security(This,ppSecurity)	\
    (This)->lpVtbl -> get_Security(This,ppSecurity)

#define IPropertyMap_put_Security(This,pSecurity)	\
    (This)->lpVtbl -> put_Security(This,pSecurity)

#define IPropertyMap_IsOfType(This,bstr,pbool)	\
    (This)->lpVtbl -> IsOfType(This,bstr,pbool)

#define IPropertyMap_ToAscii(This,pbstr)	\
    (This)->lpVtbl -> ToAscii(This,pbstr)

#define IPropertyMap_Dump(This)	\
    (This)->lpVtbl -> Dump(This)

#define IPropertyMap_Clone(This,pobjectproperty)	\
    (This)->lpVtbl -> Clone(This,pobjectproperty)

#define IPropertyMap_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define IPropertyMap_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define IPropertyMap_MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)


#define IPropertyMap_get_IsEmpty(This,pbool)	\
    (This)->lpVtbl -> get_IsEmpty(This,pbool)

#define IPropertyMap_get_Count(This,retval)	\
    (This)->lpVtbl -> get_Count(This,retval)

#define IPropertyMap_FirstItem(This,name,pvar,pbool)	\
    (This)->lpVtbl -> FirstItem(This,name,pvar,pbool)

#define IPropertyMap_NextItem(This,name,pvar,pbool)	\
    (This)->lpVtbl -> NextItem(This,name,pvar,pbool)

#define IPropertyMap_get_PropertyAtIndex(This,index,pvar)	\
    (This)->lpVtbl -> get_PropertyAtIndex(This,index,pvar)

#define IPropertyMap_get__NewEnum(This,ppunk)	\
    (This)->lpVtbl -> get__NewEnum(This,ppunk)


#define IPropertyMap_get_Property(This,bstrName,pvar)	\
    (This)->lpVtbl -> get_Property(This,bstrName,pvar)

#define IPropertyMap_put_Property(This,bstrName,var)	\
    (This)->lpVtbl -> put_Property(This,bstrName,var)

#define IPropertyMap_get_PropertyExt(This,bstrName,nHashOrig,pvar)	\
    (This)->lpVtbl -> get_PropertyExt(This,bstrName,nHashOrig,pvar)

#define IPropertyMap_put_PropertyInt(This,bstrName,var,pSecurity)	\
    (This)->lpVtbl -> put_PropertyInt(This,bstrName,var,pSecurity)

#define IPropertyMap_get_PropertySecurity(This,bstrName,ppSecurity)	\
    (This)->lpVtbl -> get_PropertySecurity(This,bstrName,ppSecurity)

#define IPropertyMap_get_PropertySecurityExt(This,bstrName,nHashOrig,ppSecurity)	\
    (This)->lpVtbl -> get_PropertySecurityExt(This,bstrName,nHashOrig,ppSecurity)

#define IPropertyMap_Remove(This,bstrName)	\
    (This)->lpVtbl -> Remove(This,bstrName)

#define IPropertyMap_RemoveInt(This,bstrName)	\
    (This)->lpVtbl -> RemoveInt(This,bstrName)

#define IPropertyMap_RemoveAll(This)	\
    (This)->lpVtbl -> RemoveAll(This)

#define IPropertyMap_get_String(This,bstrName,pbstr)	\
    (This)->lpVtbl -> get_String(This,bstrName,pbstr)

#define IPropertyMap_put_String(This,bstrName,bstr)	\
    (This)->lpVtbl -> put_String(This,bstrName,bstr)

#define IPropertyMap_get_ObjectProperty(This,bstrName,ppobjectproperty)	\
    (This)->lpVtbl -> get_ObjectProperty(This,bstrName,ppobjectproperty)

#define IPropertyMap_put_ObjectProperty(This,bstrName,pobjectproperty)	\
    (This)->lpVtbl -> put_ObjectProperty(This,bstrName,pobjectproperty)

#define IPropertyMap_get_ObjectPropertyExt(This,bstrName,iid,ppobjectproperty)	\
    (This)->lpVtbl -> get_ObjectPropertyExt(This,bstrName,iid,ppobjectproperty)

#define IPropertyMap_get_Thing(This,bstrName,ppthing)	\
    (This)->lpVtbl -> get_Thing(This,bstrName,ppthing)

#define IPropertyMap_put_Thing(This,bstrName,pthing)	\
    (This)->lpVtbl -> put_Thing(This,bstrName,pthing)

#define IPropertyMap_get_BOOL(This,bstrName,pboolVal)	\
    (This)->lpVtbl -> get_BOOL(This,bstrName,pboolVal)

#define IPropertyMap_put_BOOL(This,bstrName,boolVal)	\
    (This)->lpVtbl -> put_BOOL(This,bstrName,boolVal)

#define IPropertyMap_get_Long(This,bstrName,plval)	\
    (This)->lpVtbl -> get_Long(This,bstrName,plval)

#define IPropertyMap_put_Long(This,bstrName,lval)	\
    (This)->lpVtbl -> put_Long(This,bstrName,lval)

#define IPropertyMap_get_Short(This,bstrName,psval)	\
    (This)->lpVtbl -> get_Short(This,bstrName,psval)

#define IPropertyMap_put_Short(This,bstrName,sval)	\
    (This)->lpVtbl -> put_Short(This,bstrName,sval)

#define IPropertyMap_get_Dispatch(This,bstrName,ppdisp)	\
    (This)->lpVtbl -> get_Dispatch(This,bstrName,ppdisp)

#define IPropertyMap_put_Dispatch(This,bstrName,pdisp)	\
    (This)->lpVtbl -> put_Dispatch(This,bstrName,pdisp)

#define IPropertyMap_IsValid(This,bstrName,pbool)	\
    (This)->lpVtbl -> IsValid(This,bstrName,pbool)

#define IPropertyMap_CopyTo(This,ppropertymap)	\
    (This)->lpVtbl -> CopyTo(This,ppropertymap)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPropertyMap_get_Property_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IPropertyMap_get_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE IPropertyMap_put_Property_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ VARIANT var);


void __RPC_STUB IPropertyMap_put_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_get_PropertyExt_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ UINT nHashOrig,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IPropertyMap_get_PropertyExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_put_PropertyInt_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ VARIANT var,
    /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);


void __RPC_STUB IPropertyMap_put_PropertyInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IPropertyMap_get_PropertySecurity_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);


void __RPC_STUB IPropertyMap_get_PropertySecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_get_PropertySecurityExt_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ UINT nHashOrig,
    /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);


void __RPC_STUB IPropertyMap_get_PropertySecurityExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IPropertyMap_Remove_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IPropertyMap_Remove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_RemoveInt_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IPropertyMap_RemoveInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IPropertyMap_RemoveAll_Proxy( 
    IPropertyMap __RPC_FAR * This);


void __RPC_STUB IPropertyMap_RemoveAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_get_String_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IPropertyMap_get_String_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_put_String_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstr);


void __RPC_STUB IPropertyMap_put_String_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_get_ObjectProperty_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty);


void __RPC_STUB IPropertyMap_get_ObjectProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_put_ObjectProperty_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);


void __RPC_STUB IPropertyMap_put_ObjectProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_get_ObjectPropertyExt_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ REFIID iid,
    /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty);


void __RPC_STUB IPropertyMap_get_ObjectPropertyExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_get_Thing_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IPropertyMap_get_Thing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_put_Thing_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IPropertyMap_put_Thing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_get_BOOL_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IPropertyMap_get_BOOL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_put_BOOL_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ VARIANT_BOOL boolVal);


void __RPC_STUB IPropertyMap_put_BOOL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_get_Long_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ long __RPC_FAR *plval);


void __RPC_STUB IPropertyMap_get_Long_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_put_Long_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ long lval);


void __RPC_STUB IPropertyMap_put_Long_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_get_Short_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ short __RPC_FAR *psval);


void __RPC_STUB IPropertyMap_get_Short_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_put_Short_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ short sval);


void __RPC_STUB IPropertyMap_put_Short_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_get_Dispatch_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdisp);


void __RPC_STUB IPropertyMap_get_Dispatch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IPropertyMap_put_Dispatch_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IDispatch __RPC_FAR *pdisp);


void __RPC_STUB IPropertyMap_put_Dispatch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IPropertyMap_IsValid_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IPropertyMap_IsValid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IPropertyMap_CopyTo_Proxy( 
    IPropertyMap __RPC_FAR * This,
    /* [in] */ IPropertyMap __RPC_FAR *ppropertymap);


void __RPC_STUB IPropertyMap_CopyTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPropertyMap_INTERFACE_DEFINED__ */


#ifndef __IThing_INTERFACE_DEFINED__
#define __IThing_INTERFACE_DEFINED__

/* interface IThing */
/* [unique][helpstring][dual][uuid][object] */ 

#define	DISPID_THING_ADDPROPERTYINT	( 101 )

#define	DISPID_THING_REMOVEPROPERTY	( 102 )

#define	DISPID_THING_PROPERTY	( 103 )

#define	DISPID_THING_ADDMETHODINT	( 104 )

#define	DISPID_THING_REMOVEMETHOD	( 105 )

#define	DISPID_THING_METHOD	( 106 )

#define	DISPID_THING_INVOKEMETHODINT	( 107 )

#define	DISPID_THING_FIREEVENTINT	( 108 )

#define	DISPID_THING_EXEMPLARINT	( 110 )

#define	DISPID_THING_MOVEINTOINT	( 111 )

#define	DISPID_THING_REMOTEPROPERTY	( 112 )

#define	DISPID_THING_INSTANCEPROPERTY	( 113 )

#define	DISPID_THING_TYPEINT	( 114 )

#define	DISPID_THING_OWNERINT	( 115 )

#define	DISPID_THING_ISSTUBINT	( 116 )

#define	FIREEVENT_SELF	( 0x1 )

#define	FIREEVENT_CONTAINER	( 0x2 )

#define	FIREEVENT_CONTENT	( 0x4 )

#define	FIREEVENT_PEER	( 0x8 )

#define	FIREEVENT_DEFAULT	( 0xf )


EXTERN_C const IID IID_IThing;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D9852-A417-11CF-9BF0-0080C7A56A8A")
    IThing : public IObjectProperty
    {
    public:
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_ID( 
            /* [retval][out] */ long __RPC_FAR *id) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_ID( 
            /* [in] */ long id) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_InstanceType( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_IsExemplar( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_IsWizard( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_TypeInt( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_OwnerInt( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_IsStub( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_IsStubInt( 
            /* [in] */ VARIANT_BOOL boolval) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Exemplar( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_ExemplarInt( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FindWithin( 
            /* [in] */ BSTR bstrName,
            /* [defaultvalue][optional][in] */ VARIANT varQuiet,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE MoveIntoExt( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE MoveIntoInt( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsValidProperty( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsValidInstanceProperty( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CreateAndAddProperty( 
            /* [in] */ BSTR bstrProgID,
            /* [in] */ BSTR bstrName,
            /* [optional][in] */ VARIANT var) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddProperty( 
            /* [in] */ BSTR bstrName,
            /* [optional][in] */ VARIANT var,
            /* [defaultvalue][optional][in] */ VARIANT flags,
            /* [defaultvalue][optional][in] */ VARIANT permission,
            /* [defaultvalue][optional][in] */ VARIANT type,
            /* [defaultvalue][optional][in] */ BSTR bstrIID,
            /* [defaultvalue][optional][in] */ BSTR bstrType) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddPropertyExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT var,
            /* [in] */ long flags,
            /* [in] */ long perms,
            /* [in] */ long type,
            /* [in] */ REFIID riid,
            /* [in] */ BSTR bstrType) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE AddPropertyInt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT var,
            /* [in] */ IThing __RPC_FAR *pExemplar,
            /* [in] */ long psbits,
            /* [in] */ long perms,
            /* [in] */ long type,
            /* [in] */ BSTR bstrIID,
            /* [in] */ BSTR bstrType) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE RemoveProperty( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [id][helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_RemoteProperty( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Property( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Property( 
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_PropertyExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ UINT nHashOrig,
            /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppexemplar,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE InstanceProperty( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IThing __RPC_FAR *pExemplar) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_PropertySecurity( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_PropertySecurityExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ UINT nHashOrig,
            /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppexemplar,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pfInstance,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_InstanceProperties( 
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Properties( 
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CopyPropertiesTo( 
            /* [in] */ IPropertyMap __RPC_FAR *ppropertymap) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsValidMethod( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsValidInstanceMethod( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CreateAndAddMethod( 
            /* [in] */ long lFlags,
            /* [in] */ IModule __RPC_FAR *pmodule,
            /* [in] */ BSTR bstrName,
            /* [defaultvalue][optional][in] */ VARIANT varFuncName,
            /* [defaultvalue][optional][in] */ VARIANT varPsbits,
            /* [defaultvalue][optional][in] */ VARIANT varPermissions) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CreateAndAddMethodExt( 
            /* [in] */ long lFlags,
            /* [in] */ IModule __RPC_FAR *pmodule,
            /* [in] */ BSTR bstrFuncName,
            /* [in] */ BSTR bstrMethName,
            /* [in] */ long psbits,
            /* [in] */ long permissions) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddMethod( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IMethod __RPC_FAR *pmethod,
            /* [defaultvalue][optional][in] */ VARIANT varPsbits,
            /* [defaultvalue][optional][in] */ VARIANT varPermissions) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddMethodExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IMethod __RPC_FAR *pmethod,
            /* [in] */ long psbits,
            /* [in] */ long permissions) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE AddMethodInt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IMethod __RPC_FAR *pmethod,
            /* [in] */ IThing __RPC_FAR *pExemplar,
            /* [in] */ long psbits,
            /* [in] */ long permissions) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE RemoveMethod( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Method( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod) = 0;
        
        virtual /* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Method( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IMethod __RPC_FAR *pmethod) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_MethodExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ UINT nHashOrig,
            /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppexemplar,
            /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_MethodSecurity( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_MethodSecurityExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ UINT nHashOrig,
            /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppexemplar,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pfInstance,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_InstanceMethods( 
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Methods( 
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CopyMethodsTo( 
            /* [in] */ IPropertyMap __RPC_FAR *ppropertymap) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FireEvent( 
            /* [in] */ BSTR bstrEvent,
            /* [optional][in] */ VARIANT var) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE FireEventExt( 
            /* [in] */ BSTR bstrEvent,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ long lFlags) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE FireEventInt( 
            /* [in] */ BSTR bstrEvent,
            /* [in] */ IPropertyList __RPC_FAR *pproplist,
            /* [in] */ long lFlags) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE InvokeMethod( 
            /* [in] */ BSTR bstrMethod,
            /* [optional][in] */ VARIANT var,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE InvokeMethodInt( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrMethod,
            /* [in] */ IPropertyList __RPC_FAR *pproplist,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE InvokeMethodExt( 
            /* [in] */ BSTR bstrMethod,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE InvokeSuperMethod( 
            /* [optional][in] */ VARIANT var,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE InvokeSuperMethodExt( 
            /* [optional][in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_CurrentMethodName( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_CurrentMethodName( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Description( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Contents( 
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppproplist) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Container( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Container( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Owner( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CheckPropertiesSecurity( 
            /* [in] */ unsigned long ulFlags,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CheckMethodsSecurity( 
            /* [in] */ unsigned long ulFlags,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CheckPropertiesSecurityExt( 
            /* [in] */ unsigned long ulFlags) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CheckMethodsSecurityExt( 
            /* [in] */ unsigned long ulFlags) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CheckPropertySecurity( 
            /* [in] */ BSTR bstrName,
            /* [in] */ unsigned long ulFlags,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CheckMethodSecurity( 
            /* [in] */ BSTR bstrName,
            /* [in] */ unsigned long ulFlags,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CheckPropertySecurityExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ unsigned long ulFlags) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CheckMethodSecurityExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ unsigned long ulFlags) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE NotifyObjectPropertyChanged( 
            /* [in] */ IObjectProperty __RPC_FAR *pprop,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE NotifyMethodChanged( 
            /* [in] */ IMethod __RPC_FAR *pmethod,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddString( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddObjectProperty( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddObjectPropertyExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ REFIID iid,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddThing( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddBOOL( 
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT_BOOL boolVal) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddLong( 
            /* [in] */ BSTR bstrName,
            /* [in] */ long lval) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddShort( 
            /* [in] */ BSTR bstrName,
            /* [in] */ short sval) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddDispatch( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IDispatch __RPC_FAR *pdisp) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_String( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_String( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ObjectProperty( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_ObjectProperty( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ObjectPropertyExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ REFIID iid,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Thing( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Thing( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_BOOL( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_BOOL( 
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT_BOOL boolVal) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Long( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ long __RPC_FAR *plval) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Long( 
            /* [in] */ BSTR bstrName,
            /* [in] */ long lval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Short( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ short __RPC_FAR *psval) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Short( 
            /* [in] */ BSTR bstrName,
            /* [in] */ short sval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Dispatch( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdisp) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Dispatch( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IDispatch __RPC_FAR *pdisp) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IThingVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IThing __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IThing __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IThing __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IThing __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IThing __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IThing __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IThing __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IThing __RPC_FAR * This);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RefCount )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CLSID )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IID )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            IThing __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parent )( 
            IThing __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyName )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyName )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Security )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Security )( 
            IThing __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsOfType )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToAscii )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            IThing __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IThing __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            IThing __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            IThing __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallToScript )( 
            IThing __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *id);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ID )( 
            IThing __RPC_FAR * This,
            /* [in] */ long id);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InstanceType )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsExemplar )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsWizard )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TypeInt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_OwnerInt )( 
            IThing __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsStub )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_IsStubInt )( 
            IThing __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL boolval);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Exemplar )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ExemplarInt )( 
            IThing __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindWithin )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [defaultvalue][optional][in] */ VARIANT varQuiet,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MoveIntoExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MoveIntoInt )( 
            IThing __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValidProperty )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValidInstanceProperty )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateAndAddProperty )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrProgID,
            /* [in] */ BSTR bstrName,
            /* [optional][in] */ VARIANT var);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddProperty )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [optional][in] */ VARIANT var,
            /* [defaultvalue][optional][in] */ VARIANT flags,
            /* [defaultvalue][optional][in] */ VARIANT permission,
            /* [defaultvalue][optional][in] */ VARIANT type,
            /* [defaultvalue][optional][in] */ BSTR bstrIID,
            /* [defaultvalue][optional][in] */ BSTR bstrType);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddPropertyExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT var,
            /* [in] */ long flags,
            /* [in] */ long perms,
            /* [in] */ long type,
            /* [in] */ REFIID riid,
            /* [in] */ BSTR bstrType);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddPropertyInt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT var,
            /* [in] */ IThing __RPC_FAR *pExemplar,
            /* [in] */ long psbits,
            /* [in] */ long perms,
            /* [in] */ long type,
            /* [in] */ BSTR bstrIID,
            /* [in] */ BSTR bstrType);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveProperty )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [id][helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RemoteProperty )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Property )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Property )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT var);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ UINT nHashOrig,
            /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppexemplar,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InstanceProperty )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IThing __RPC_FAR *pExemplar);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertySecurity )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertySecurityExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ UINT nHashOrig,
            /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppexemplar,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pfInstance,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InstanceProperties )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Properties )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CopyPropertiesTo )( 
            IThing __RPC_FAR * This,
            /* [in] */ IPropertyMap __RPC_FAR *ppropertymap);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValidMethod )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValidInstanceMethod )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateAndAddMethod )( 
            IThing __RPC_FAR * This,
            /* [in] */ long lFlags,
            /* [in] */ IModule __RPC_FAR *pmodule,
            /* [in] */ BSTR bstrName,
            /* [defaultvalue][optional][in] */ VARIANT varFuncName,
            /* [defaultvalue][optional][in] */ VARIANT varPsbits,
            /* [defaultvalue][optional][in] */ VARIANT varPermissions);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateAndAddMethodExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ long lFlags,
            /* [in] */ IModule __RPC_FAR *pmodule,
            /* [in] */ BSTR bstrFuncName,
            /* [in] */ BSTR bstrMethName,
            /* [in] */ long psbits,
            /* [in] */ long permissions);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddMethod )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IMethod __RPC_FAR *pmethod,
            /* [defaultvalue][optional][in] */ VARIANT varPsbits,
            /* [defaultvalue][optional][in] */ VARIANT varPermissions);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddMethodExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IMethod __RPC_FAR *pmethod,
            /* [in] */ long psbits,
            /* [in] */ long permissions);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddMethodInt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IMethod __RPC_FAR *pmethod,
            /* [in] */ IThing __RPC_FAR *pExemplar,
            /* [in] */ long psbits,
            /* [in] */ long permissions);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveMethod )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Method )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Method )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IMethod __RPC_FAR *pmethod);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MethodExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ UINT nHashOrig,
            /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppexemplar,
            /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MethodSecurity )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MethodSecurityExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ UINT nHashOrig,
            /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppexemplar,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pfInstance,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InstanceMethods )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Methods )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CopyMethodsTo )( 
            IThing __RPC_FAR * This,
            /* [in] */ IPropertyMap __RPC_FAR *ppropertymap);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FireEvent )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrEvent,
            /* [optional][in] */ VARIANT var);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FireEventExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrEvent,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ long lFlags);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FireEventInt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrEvent,
            /* [in] */ IPropertyList __RPC_FAR *pproplist,
            /* [in] */ long lFlags);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeMethod )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrMethod,
            /* [optional][in] */ VARIANT var,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeMethodInt )( 
            IThing __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrMethod,
            /* [in] */ IPropertyList __RPC_FAR *pproplist,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeMethodExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrMethod,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeSuperMethod )( 
            IThing __RPC_FAR * This,
            /* [optional][in] */ VARIANT var,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeSuperMethodExt )( 
            IThing __RPC_FAR * This,
            /* [optional][in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CurrentMethodName )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_CurrentMethodName )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Description )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Description )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Contents )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppproplist);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Container )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Container )( 
            IThing __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Owner )( 
            IThing __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckPropertiesSecurity )( 
            IThing __RPC_FAR * This,
            /* [in] */ unsigned long ulFlags,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckMethodsSecurity )( 
            IThing __RPC_FAR * This,
            /* [in] */ unsigned long ulFlags,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckPropertiesSecurityExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ unsigned long ulFlags);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckMethodsSecurityExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ unsigned long ulFlags);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckPropertySecurity )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ unsigned long ulFlags,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckMethodSecurity )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ unsigned long ulFlags,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckPropertySecurityExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ unsigned long ulFlags);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckMethodSecurityExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ unsigned long ulFlags);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NotifyObjectPropertyChanged )( 
            IThing __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pprop,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NotifyMethodChanged )( 
            IThing __RPC_FAR * This,
            /* [in] */ IMethod __RPC_FAR *pmethod,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddString )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddObjectProperty )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddObjectPropertyExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ REFIID iid,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddThing )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddBOOL )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT_BOOL boolVal);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddLong )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ long lval);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddShort )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ short sval);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddDispatch )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IDispatch __RPC_FAR *pdisp);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_String )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_String )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ObjectProperty )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ObjectProperty )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ObjectPropertyExt )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ REFIID iid,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Thing )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Thing )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BOOL )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BOOL )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT_BOOL boolVal);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Long )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ long __RPC_FAR *plval);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Long )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ long lval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Short )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ short __RPC_FAR *psval);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Short )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ short sval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dispatch )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdisp);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Dispatch )( 
            IThing __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IDispatch __RPC_FAR *pdisp);
        
        END_INTERFACE
    } IThingVtbl;

    interface IThing
    {
        CONST_VTBL struct IThingVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IThing_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IThing_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IThing_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IThing_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IThing_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IThing_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IThing_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IThing_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IThing_get_RefCount(This,pdwRef)	\
    (This)->lpVtbl -> get_RefCount(This,pdwRef)

#define IThing_get_CLSID(This,pclsid)	\
    (This)->lpVtbl -> get_CLSID(This,pclsid)

#define IThing_get_IID(This,piid)	\
    (This)->lpVtbl -> get_IID(This,piid)

#define IThing_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IThing_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define IThing_get_Parent(This,ppthing)	\
    (This)->lpVtbl -> get_Parent(This,ppthing)

#define IThing_put_Parent(This,pthing)	\
    (This)->lpVtbl -> put_Parent(This,pthing)

#define IThing_get_PropertyName(This,pbstr)	\
    (This)->lpVtbl -> get_PropertyName(This,pbstr)

#define IThing_put_PropertyName(This,bstr)	\
    (This)->lpVtbl -> put_PropertyName(This,bstr)

#define IThing_get_Type(This,pbstr)	\
    (This)->lpVtbl -> get_Type(This,pbstr)

#define IThing_put_Type(This,bstr)	\
    (This)->lpVtbl -> put_Type(This,bstr)

#define IThing_get_Security(This,ppSecurity)	\
    (This)->lpVtbl -> get_Security(This,ppSecurity)

#define IThing_put_Security(This,pSecurity)	\
    (This)->lpVtbl -> put_Security(This,pSecurity)

#define IThing_IsOfType(This,bstr,pbool)	\
    (This)->lpVtbl -> IsOfType(This,bstr,pbool)

#define IThing_ToAscii(This,pbstr)	\
    (This)->lpVtbl -> ToAscii(This,pbstr)

#define IThing_Dump(This)	\
    (This)->lpVtbl -> Dump(This)

#define IThing_Clone(This,pobjectproperty)	\
    (This)->lpVtbl -> Clone(This,pobjectproperty)

#define IThing_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define IThing_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define IThing_MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)


#define IThing_get_ID(This,id)	\
    (This)->lpVtbl -> get_ID(This,id)

#define IThing_put_ID(This,id)	\
    (This)->lpVtbl -> put_ID(This,id)

#define IThing_get_InstanceType(This,pbstr)	\
    (This)->lpVtbl -> get_InstanceType(This,pbstr)

#define IThing_get_IsExemplar(This,pbool)	\
    (This)->lpVtbl -> get_IsExemplar(This,pbool)

#define IThing_get_IsWizard(This,pbool)	\
    (This)->lpVtbl -> get_IsWizard(This,pbool)

#define IThing_put_TypeInt(This,bstr)	\
    (This)->lpVtbl -> put_TypeInt(This,bstr)

#define IThing_put_OwnerInt(This,pThing)	\
    (This)->lpVtbl -> put_OwnerInt(This,pThing)

#define IThing_get_IsStub(This,pbool)	\
    (This)->lpVtbl -> get_IsStub(This,pbool)

#define IThing_put_IsStubInt(This,boolval)	\
    (This)->lpVtbl -> put_IsStubInt(This,boolval)

#define IThing_get_Exemplar(This,ppthing)	\
    (This)->lpVtbl -> get_Exemplar(This,ppthing)

#define IThing_put_ExemplarInt(This,pthing)	\
    (This)->lpVtbl -> put_ExemplarInt(This,pthing)

#define IThing_FindWithin(This,bstrName,varQuiet,ppthing)	\
    (This)->lpVtbl -> FindWithin(This,bstrName,varQuiet,ppthing)

#define IThing_MoveIntoExt(This,pthing)	\
    (This)->lpVtbl -> MoveIntoExt(This,pthing)

#define IThing_MoveIntoInt(This,pthing)	\
    (This)->lpVtbl -> MoveIntoInt(This,pthing)

#define IThing_IsValidProperty(This,bstrName,pbool)	\
    (This)->lpVtbl -> IsValidProperty(This,bstrName,pbool)

#define IThing_IsValidInstanceProperty(This,bstrName,pbool)	\
    (This)->lpVtbl -> IsValidInstanceProperty(This,bstrName,pbool)

#define IThing_CreateAndAddProperty(This,bstrProgID,bstrName,var)	\
    (This)->lpVtbl -> CreateAndAddProperty(This,bstrProgID,bstrName,var)

#define IThing_AddProperty(This,bstrName,var,flags,permission,type,bstrIID,bstrType)	\
    (This)->lpVtbl -> AddProperty(This,bstrName,var,flags,permission,type,bstrIID,bstrType)

#define IThing_AddPropertyExt(This,bstrName,var,flags,perms,type,riid,bstrType)	\
    (This)->lpVtbl -> AddPropertyExt(This,bstrName,var,flags,perms,type,riid,bstrType)

#define IThing_AddPropertyInt(This,bstrName,var,pExemplar,psbits,perms,type,bstrIID,bstrType)	\
    (This)->lpVtbl -> AddPropertyInt(This,bstrName,var,pExemplar,psbits,perms,type,bstrIID,bstrType)

#define IThing_RemoveProperty(This,bstrName)	\
    (This)->lpVtbl -> RemoveProperty(This,bstrName)

#define IThing_get_RemoteProperty(This,bstrName,pvar)	\
    (This)->lpVtbl -> get_RemoteProperty(This,bstrName,pvar)

#define IThing_get_Property(This,bstrName,pvar)	\
    (This)->lpVtbl -> get_Property(This,bstrName,pvar)

#define IThing_put_Property(This,bstrName,var)	\
    (This)->lpVtbl -> put_Property(This,bstrName,var)

#define IThing_get_PropertyExt(This,bstrName,nHashOrig,ppexemplar,pvar)	\
    (This)->lpVtbl -> get_PropertyExt(This,bstrName,nHashOrig,ppexemplar,pvar)

#define IThing_InstanceProperty(This,bstrName,pExemplar)	\
    (This)->lpVtbl -> InstanceProperty(This,bstrName,pExemplar)

#define IThing_get_PropertySecurity(This,bstrName,ppSecurity)	\
    (This)->lpVtbl -> get_PropertySecurity(This,bstrName,ppSecurity)

#define IThing_get_PropertySecurityExt(This,bstrName,nHashOrig,ppexemplar,pfInstance,ppSecurity)	\
    (This)->lpVtbl -> get_PropertySecurityExt(This,bstrName,nHashOrig,ppexemplar,pfInstance,ppSecurity)

#define IThing_get_InstanceProperties(This,pppropertymap)	\
    (This)->lpVtbl -> get_InstanceProperties(This,pppropertymap)

#define IThing_get_Properties(This,pppropertymap)	\
    (This)->lpVtbl -> get_Properties(This,pppropertymap)

#define IThing_CopyPropertiesTo(This,ppropertymap)	\
    (This)->lpVtbl -> CopyPropertiesTo(This,ppropertymap)

#define IThing_IsValidMethod(This,bstrName,pbool)	\
    (This)->lpVtbl -> IsValidMethod(This,bstrName,pbool)

#define IThing_IsValidInstanceMethod(This,bstrName,pbool)	\
    (This)->lpVtbl -> IsValidInstanceMethod(This,bstrName,pbool)

#define IThing_CreateAndAddMethod(This,lFlags,pmodule,bstrName,varFuncName,varPsbits,varPermissions)	\
    (This)->lpVtbl -> CreateAndAddMethod(This,lFlags,pmodule,bstrName,varFuncName,varPsbits,varPermissions)

#define IThing_CreateAndAddMethodExt(This,lFlags,pmodule,bstrFuncName,bstrMethName,psbits,permissions)	\
    (This)->lpVtbl -> CreateAndAddMethodExt(This,lFlags,pmodule,bstrFuncName,bstrMethName,psbits,permissions)

#define IThing_AddMethod(This,bstrName,pmethod,varPsbits,varPermissions)	\
    (This)->lpVtbl -> AddMethod(This,bstrName,pmethod,varPsbits,varPermissions)

#define IThing_AddMethodExt(This,bstrName,pmethod,psbits,permissions)	\
    (This)->lpVtbl -> AddMethodExt(This,bstrName,pmethod,psbits,permissions)

#define IThing_AddMethodInt(This,bstrName,pmethod,pExemplar,psbits,permissions)	\
    (This)->lpVtbl -> AddMethodInt(This,bstrName,pmethod,pExemplar,psbits,permissions)

#define IThing_RemoveMethod(This,bstrName)	\
    (This)->lpVtbl -> RemoveMethod(This,bstrName)

#define IThing_get_Method(This,bstrName,ppmethod)	\
    (This)->lpVtbl -> get_Method(This,bstrName,ppmethod)

#define IThing_put_Method(This,bstrName,pmethod)	\
    (This)->lpVtbl -> put_Method(This,bstrName,pmethod)

#define IThing_get_MethodExt(This,bstrName,nHashOrig,ppexemplar,ppmethod)	\
    (This)->lpVtbl -> get_MethodExt(This,bstrName,nHashOrig,ppexemplar,ppmethod)

#define IThing_get_MethodSecurity(This,bstrName,ppSecurity)	\
    (This)->lpVtbl -> get_MethodSecurity(This,bstrName,ppSecurity)

#define IThing_get_MethodSecurityExt(This,bstrName,nHashOrig,ppexemplar,pfInstance,ppSecurity)	\
    (This)->lpVtbl -> get_MethodSecurityExt(This,bstrName,nHashOrig,ppexemplar,pfInstance,ppSecurity)

#define IThing_get_InstanceMethods(This,pppropertymap)	\
    (This)->lpVtbl -> get_InstanceMethods(This,pppropertymap)

#define IThing_get_Methods(This,pppropertymap)	\
    (This)->lpVtbl -> get_Methods(This,pppropertymap)

#define IThing_CopyMethodsTo(This,ppropertymap)	\
    (This)->lpVtbl -> CopyMethodsTo(This,ppropertymap)

#define IThing_FireEvent(This,bstrEvent,var)	\
    (This)->lpVtbl -> FireEvent(This,bstrEvent,var)

#define IThing_FireEventExt(This,bstrEvent,pdispparams,lFlags)	\
    (This)->lpVtbl -> FireEventExt(This,bstrEvent,pdispparams,lFlags)

#define IThing_FireEventInt(This,bstrEvent,pproplist,lFlags)	\
    (This)->lpVtbl -> FireEventInt(This,bstrEvent,pproplist,lFlags)

#define IThing_InvokeMethod(This,bstrMethod,var,pvar)	\
    (This)->lpVtbl -> InvokeMethod(This,bstrMethod,var,pvar)

#define IThing_InvokeMethodInt(This,pThing,bstrMethod,pproplist,pvar)	\
    (This)->lpVtbl -> InvokeMethodInt(This,pThing,bstrMethod,pproplist,pvar)

#define IThing_InvokeMethodExt(This,bstrMethod,pdispparams,pvar)	\
    (This)->lpVtbl -> InvokeMethodExt(This,bstrMethod,pdispparams,pvar)

#define IThing_InvokeSuperMethod(This,var,pvar)	\
    (This)->lpVtbl -> InvokeSuperMethod(This,var,pvar)

#define IThing_InvokeSuperMethodExt(This,pdispparams,pvar)	\
    (This)->lpVtbl -> InvokeSuperMethodExt(This,pdispparams,pvar)

#define IThing_get_CurrentMethodName(This,pbstr)	\
    (This)->lpVtbl -> get_CurrentMethodName(This,pbstr)

#define IThing_put_CurrentMethodName(This,bstr)	\
    (This)->lpVtbl -> put_CurrentMethodName(This,bstr)

#define IThing_get_Name(This,pbstr)	\
    (This)->lpVtbl -> get_Name(This,pbstr)

#define IThing_put_Name(This,bstr)	\
    (This)->lpVtbl -> put_Name(This,bstr)

#define IThing_get_Description(This,pbstr)	\
    (This)->lpVtbl -> get_Description(This,pbstr)

#define IThing_put_Description(This,bstr)	\
    (This)->lpVtbl -> put_Description(This,bstr)

#define IThing_get_Contents(This,ppproplist)	\
    (This)->lpVtbl -> get_Contents(This,ppproplist)

#define IThing_get_Container(This,ppthing)	\
    (This)->lpVtbl -> get_Container(This,ppthing)

#define IThing_put_Container(This,pthing)	\
    (This)->lpVtbl -> put_Container(This,pthing)

#define IThing_get_Owner(This,ppthing)	\
    (This)->lpVtbl -> get_Owner(This,ppthing)

#define IThing_CheckPropertiesSecurity(This,ulFlags,pboolVal)	\
    (This)->lpVtbl -> CheckPropertiesSecurity(This,ulFlags,pboolVal)

#define IThing_CheckMethodsSecurity(This,ulFlags,pboolVal)	\
    (This)->lpVtbl -> CheckMethodsSecurity(This,ulFlags,pboolVal)

#define IThing_CheckPropertiesSecurityExt(This,ulFlags)	\
    (This)->lpVtbl -> CheckPropertiesSecurityExt(This,ulFlags)

#define IThing_CheckMethodsSecurityExt(This,ulFlags)	\
    (This)->lpVtbl -> CheckMethodsSecurityExt(This,ulFlags)

#define IThing_CheckPropertySecurity(This,bstrName,ulFlags,pboolVal)	\
    (This)->lpVtbl -> CheckPropertySecurity(This,bstrName,ulFlags,pboolVal)

#define IThing_CheckMethodSecurity(This,bstrName,ulFlags,pboolVal)	\
    (This)->lpVtbl -> CheckMethodSecurity(This,bstrName,ulFlags,pboolVal)

#define IThing_CheckPropertySecurityExt(This,bstrName,ulFlags)	\
    (This)->lpVtbl -> CheckPropertySecurityExt(This,bstrName,ulFlags)

#define IThing_CheckMethodSecurityExt(This,bstrName,ulFlags)	\
    (This)->lpVtbl -> CheckMethodSecurityExt(This,bstrName,ulFlags)

#define IThing_NotifyObjectPropertyChanged(This,pprop,lHint,varHintData)	\
    (This)->lpVtbl -> NotifyObjectPropertyChanged(This,pprop,lHint,varHintData)

#define IThing_NotifyMethodChanged(This,pmethod,lHint,varHintData)	\
    (This)->lpVtbl -> NotifyMethodChanged(This,pmethod,lHint,varHintData)

#define IThing_AddString(This,bstrName,bstr)	\
    (This)->lpVtbl -> AddString(This,bstrName,bstr)

#define IThing_AddObjectProperty(This,bstrName,pobjectproperty)	\
    (This)->lpVtbl -> AddObjectProperty(This,bstrName,pobjectproperty)

#define IThing_AddObjectPropertyExt(This,bstrName,iid,pobjectproperty)	\
    (This)->lpVtbl -> AddObjectPropertyExt(This,bstrName,iid,pobjectproperty)

#define IThing_AddThing(This,bstrName,pthing)	\
    (This)->lpVtbl -> AddThing(This,bstrName,pthing)

#define IThing_AddBOOL(This,bstrName,boolVal)	\
    (This)->lpVtbl -> AddBOOL(This,bstrName,boolVal)

#define IThing_AddLong(This,bstrName,lval)	\
    (This)->lpVtbl -> AddLong(This,bstrName,lval)

#define IThing_AddShort(This,bstrName,sval)	\
    (This)->lpVtbl -> AddShort(This,bstrName,sval)

#define IThing_AddDispatch(This,bstrName,pdisp)	\
    (This)->lpVtbl -> AddDispatch(This,bstrName,pdisp)

#define IThing_get_String(This,bstrName,pbstr)	\
    (This)->lpVtbl -> get_String(This,bstrName,pbstr)

#define IThing_put_String(This,bstrName,bstr)	\
    (This)->lpVtbl -> put_String(This,bstrName,bstr)

#define IThing_get_ObjectProperty(This,bstrName,ppobjectproperty)	\
    (This)->lpVtbl -> get_ObjectProperty(This,bstrName,ppobjectproperty)

#define IThing_put_ObjectProperty(This,bstrName,pobjectproperty)	\
    (This)->lpVtbl -> put_ObjectProperty(This,bstrName,pobjectproperty)

#define IThing_get_ObjectPropertyExt(This,bstrName,iid,ppobjectproperty)	\
    (This)->lpVtbl -> get_ObjectPropertyExt(This,bstrName,iid,ppobjectproperty)

#define IThing_get_Thing(This,bstrName,ppthing)	\
    (This)->lpVtbl -> get_Thing(This,bstrName,ppthing)

#define IThing_put_Thing(This,bstrName,pthing)	\
    (This)->lpVtbl -> put_Thing(This,bstrName,pthing)

#define IThing_get_BOOL(This,bstrName,pboolVal)	\
    (This)->lpVtbl -> get_BOOL(This,bstrName,pboolVal)

#define IThing_put_BOOL(This,bstrName,boolVal)	\
    (This)->lpVtbl -> put_BOOL(This,bstrName,boolVal)

#define IThing_get_Long(This,bstrName,plval)	\
    (This)->lpVtbl -> get_Long(This,bstrName,plval)

#define IThing_put_Long(This,bstrName,lval)	\
    (This)->lpVtbl -> put_Long(This,bstrName,lval)

#define IThing_get_Short(This,bstrName,psval)	\
    (This)->lpVtbl -> get_Short(This,bstrName,psval)

#define IThing_put_Short(This,bstrName,sval)	\
    (This)->lpVtbl -> put_Short(This,bstrName,sval)

#define IThing_get_Dispatch(This,bstrName,ppdisp)	\
    (This)->lpVtbl -> get_Dispatch(This,bstrName,ppdisp)

#define IThing_put_Dispatch(This,bstrName,pdisp)	\
    (This)->lpVtbl -> put_Dispatch(This,bstrName,pdisp)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IThing_get_ID_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *id);


void __RPC_STUB IThing_get_ID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IThing_put_ID_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ long id);


void __RPC_STUB IThing_put_ID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_InstanceType_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IThing_get_InstanceType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IThing_get_IsExemplar_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IThing_get_IsExemplar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_IsWizard_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IThing_get_IsWizard_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IThing_put_TypeInt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IThing_put_TypeInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IThing_put_OwnerInt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB IThing_put_OwnerInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IThing_get_IsStub_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IThing_get_IsStub_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IThing_put_IsStubInt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL boolval);


void __RPC_STUB IThing_put_IsStubInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IThing_get_Exemplar_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IThing_get_Exemplar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IThing_put_ExemplarInt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IThing_put_ExemplarInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThing_FindWithin_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [defaultvalue][optional][in] */ VARIANT varQuiet,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IThing_FindWithin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_MoveIntoExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IThing_MoveIntoExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThing_MoveIntoInt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IThing_MoveIntoInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThing_IsValidProperty_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IThing_IsValidProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThing_IsValidInstanceProperty_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IThing_IsValidInstanceProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThing_CreateAndAddProperty_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrProgID,
    /* [in] */ BSTR bstrName,
    /* [optional][in] */ VARIANT var);


void __RPC_STUB IThing_CreateAndAddProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThing_AddProperty_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [optional][in] */ VARIANT var,
    /* [defaultvalue][optional][in] */ VARIANT flags,
    /* [defaultvalue][optional][in] */ VARIANT permission,
    /* [defaultvalue][optional][in] */ VARIANT type,
    /* [defaultvalue][optional][in] */ BSTR bstrIID,
    /* [defaultvalue][optional][in] */ BSTR bstrType);


void __RPC_STUB IThing_AddProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_AddPropertyExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ VARIANT var,
    /* [in] */ long flags,
    /* [in] */ long perms,
    /* [in] */ long type,
    /* [in] */ REFIID riid,
    /* [in] */ BSTR bstrType);


void __RPC_STUB IThing_AddPropertyExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThing_AddPropertyInt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ VARIANT var,
    /* [in] */ IThing __RPC_FAR *pExemplar,
    /* [in] */ long psbits,
    /* [in] */ long perms,
    /* [in] */ long type,
    /* [in] */ BSTR bstrIID,
    /* [in] */ BSTR bstrType);


void __RPC_STUB IThing_AddPropertyInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IThing_RemoveProperty_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IThing_RemoveProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IThing_get_RemoteProperty_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IThing_get_RemoteProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IThing_get_Property_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IThing_get_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE IThing_put_Property_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ VARIANT var);


void __RPC_STUB IThing_put_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_PropertyExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ UINT nHashOrig,
    /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppexemplar,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IThing_get_PropertyExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThing_InstanceProperty_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IThing __RPC_FAR *pExemplar);


void __RPC_STUB IThing_InstanceProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IThing_get_PropertySecurity_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);


void __RPC_STUB IThing_get_PropertySecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_PropertySecurityExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ UINT nHashOrig,
    /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppexemplar,
    /* [out] */ VARIANT_BOOL __RPC_FAR *pfInstance,
    /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);


void __RPC_STUB IThing_get_PropertySecurityExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IThing_get_InstanceProperties_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap);


void __RPC_STUB IThing_get_InstanceProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_Properties_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap);


void __RPC_STUB IThing_get_Properties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_CopyPropertiesTo_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ IPropertyMap __RPC_FAR *ppropertymap);


void __RPC_STUB IThing_CopyPropertiesTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThing_IsValidMethod_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IThing_IsValidMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThing_IsValidInstanceMethod_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IThing_IsValidInstanceMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThing_CreateAndAddMethod_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ long lFlags,
    /* [in] */ IModule __RPC_FAR *pmodule,
    /* [in] */ BSTR bstrName,
    /* [defaultvalue][optional][in] */ VARIANT varFuncName,
    /* [defaultvalue][optional][in] */ VARIANT varPsbits,
    /* [defaultvalue][optional][in] */ VARIANT varPermissions);


void __RPC_STUB IThing_CreateAndAddMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_CreateAndAddMethodExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ long lFlags,
    /* [in] */ IModule __RPC_FAR *pmodule,
    /* [in] */ BSTR bstrFuncName,
    /* [in] */ BSTR bstrMethName,
    /* [in] */ long psbits,
    /* [in] */ long permissions);


void __RPC_STUB IThing_CreateAndAddMethodExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThing_AddMethod_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IMethod __RPC_FAR *pmethod,
    /* [defaultvalue][optional][in] */ VARIANT varPsbits,
    /* [defaultvalue][optional][in] */ VARIANT varPermissions);


void __RPC_STUB IThing_AddMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_AddMethodExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IMethod __RPC_FAR *pmethod,
    /* [in] */ long psbits,
    /* [in] */ long permissions);


void __RPC_STUB IThing_AddMethodExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThing_AddMethodInt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IMethod __RPC_FAR *pmethod,
    /* [in] */ IThing __RPC_FAR *pExemplar,
    /* [in] */ long psbits,
    /* [in] */ long permissions);


void __RPC_STUB IThing_AddMethodInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IThing_RemoveMethod_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IThing_RemoveMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IThing_get_Method_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod);


void __RPC_STUB IThing_get_Method_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE IThing_put_Method_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IMethod __RPC_FAR *pmethod);


void __RPC_STUB IThing_put_Method_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_MethodExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ UINT nHashOrig,
    /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppexemplar,
    /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod);


void __RPC_STUB IThing_get_MethodExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IThing_get_MethodSecurity_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);


void __RPC_STUB IThing_get_MethodSecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_MethodSecurityExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ UINT nHashOrig,
    /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppexemplar,
    /* [out] */ VARIANT_BOOL __RPC_FAR *pfInstance,
    /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);


void __RPC_STUB IThing_get_MethodSecurityExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IThing_get_InstanceMethods_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap);


void __RPC_STUB IThing_get_InstanceMethods_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_Methods_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap);


void __RPC_STUB IThing_get_Methods_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_CopyMethodsTo_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ IPropertyMap __RPC_FAR *ppropertymap);


void __RPC_STUB IThing_CopyMethodsTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThing_FireEvent_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrEvent,
    /* [optional][in] */ VARIANT var);


void __RPC_STUB IThing_FireEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_FireEventExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrEvent,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [in] */ long lFlags);


void __RPC_STUB IThing_FireEventExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThing_FireEventInt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrEvent,
    /* [in] */ IPropertyList __RPC_FAR *pproplist,
    /* [in] */ long lFlags);


void __RPC_STUB IThing_FireEventInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThing_InvokeMethod_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrMethod,
    /* [optional][in] */ VARIANT var,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IThing_InvokeMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThing_InvokeMethodInt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrMethod,
    /* [in] */ IPropertyList __RPC_FAR *pproplist,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IThing_InvokeMethodInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_InvokeMethodExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrMethod,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IThing_InvokeMethodExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThing_InvokeSuperMethod_Proxy( 
    IThing __RPC_FAR * This,
    /* [optional][in] */ VARIANT var,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IThing_InvokeSuperMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_InvokeSuperMethodExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [optional][in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IThing_InvokeSuperMethodExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_CurrentMethodName_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IThing_get_CurrentMethodName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IThing_put_CurrentMethodName_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IThing_put_CurrentMethodName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_Name_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IThing_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IThing_put_Name_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IThing_put_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_Description_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IThing_get_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IThing_put_Description_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IThing_put_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_Contents_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppproplist);


void __RPC_STUB IThing_get_Contents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_Container_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IThing_get_Container_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IThing_put_Container_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IThing_put_Container_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IThing_get_Owner_Proxy( 
    IThing __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IThing_get_Owner_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThing_CheckPropertiesSecurity_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ unsigned long ulFlags,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IThing_CheckPropertiesSecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThing_CheckMethodsSecurity_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ unsigned long ulFlags,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IThing_CheckMethodsSecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_CheckPropertiesSecurityExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ unsigned long ulFlags);


void __RPC_STUB IThing_CheckPropertiesSecurityExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_CheckMethodsSecurityExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ unsigned long ulFlags);


void __RPC_STUB IThing_CheckMethodsSecurityExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThing_CheckPropertySecurity_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ unsigned long ulFlags,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IThing_CheckPropertySecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThing_CheckMethodSecurity_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ unsigned long ulFlags,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IThing_CheckMethodSecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_CheckPropertySecurityExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ unsigned long ulFlags);


void __RPC_STUB IThing_CheckPropertySecurityExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_CheckMethodSecurityExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ unsigned long ulFlags);


void __RPC_STUB IThing_CheckMethodSecurityExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_NotifyObjectPropertyChanged_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ IObjectProperty __RPC_FAR *pprop,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData);


void __RPC_STUB IThing_NotifyObjectPropertyChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_NotifyMethodChanged_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ IMethod __RPC_FAR *pmethod,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData);


void __RPC_STUB IThing_NotifyMethodChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_AddString_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstr);


void __RPC_STUB IThing_AddString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_AddObjectProperty_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);


void __RPC_STUB IThing_AddObjectProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_AddObjectPropertyExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ REFIID iid,
    /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);


void __RPC_STUB IThing_AddObjectPropertyExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_AddThing_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IThing_AddThing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_AddBOOL_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ VARIANT_BOOL boolVal);


void __RPC_STUB IThing_AddBOOL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_AddLong_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ long lval);


void __RPC_STUB IThing_AddLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_AddShort_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ short sval);


void __RPC_STUB IThing_AddShort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IThing_AddDispatch_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IDispatch __RPC_FAR *pdisp);


void __RPC_STUB IThing_AddDispatch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_String_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IThing_get_String_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IThing_put_String_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstr);


void __RPC_STUB IThing_put_String_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_ObjectProperty_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty);


void __RPC_STUB IThing_get_ObjectProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IThing_put_ObjectProperty_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);


void __RPC_STUB IThing_put_ObjectProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_ObjectPropertyExt_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ REFIID iid,
    /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppobjectproperty);


void __RPC_STUB IThing_get_ObjectPropertyExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_Thing_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IThing_get_Thing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IThing_put_Thing_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IThing_put_Thing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_BOOL_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IThing_get_BOOL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IThing_put_BOOL_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ VARIANT_BOOL boolVal);


void __RPC_STUB IThing_put_BOOL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_Long_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ long __RPC_FAR *plval);


void __RPC_STUB IThing_get_Long_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IThing_put_Long_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ long lval);


void __RPC_STUB IThing_put_Long_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_Short_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ short __RPC_FAR *psval);


void __RPC_STUB IThing_get_Short_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IThing_put_Short_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ short sval);


void __RPC_STUB IThing_put_Short_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IThing_get_Dispatch_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdisp);


void __RPC_STUB IThing_get_Dispatch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IThing_put_Dispatch_Proxy( 
    IThing __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IDispatch __RPC_FAR *pdisp);


void __RPC_STUB IThing_put_Dispatch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IThing_INTERFACE_DEFINED__ */


#ifndef __IModule_INTERFACE_DEFINED__
#define __IModule_INTERFACE_DEFINED__

/* interface IModule */
/* [unique][helpstring][dual][uuid][object] */ 

#define	DISPID_MODULE_FLAGS	( 101 )

#define	DISPID_MODULE_MODULENAME	( 102 )

#define	DISPID_MODULE_RELOAD	( 103 )


EXTERN_C const IID IID_IModule;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D984A-A417-11CF-9BF0-0080C7A56A8A")
    IModule : public IObjectProperty
    {
    public:
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ModuleName( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_ModuleType( 
            /* [retval][out] */ long __RPC_FAR *plval) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Flags( 
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_Flags( 
            /* [in] */ long lVal) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Install( void) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE InvokeByName( 
            /* [in] */ BSTR bstrSubName,
            /* [in] */ WORD wFlags,
            /* [out] */ DISPID __RPC_FAR *pdispid,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ VARIANT_BOOL bQuiet,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [out] */ HRESULT __RPC_FAR *phr) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE InvokeByDispID( 
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ VARIANT_BOOL bQuiet,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [out] */ HRESULT __RPC_FAR *phr) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Dispatch( 
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispatch) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE Reload( 
            /* [in] */ VARIANT_BOOL bReinstall) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Unload( 
            /* [in] */ VARIANT_BOOL bShutdown) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Validate( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_IsShutdown( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IModuleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IModule __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IModule __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IModule __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IModule __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IModule __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IModule __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IModule __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IModule __RPC_FAR * This);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RefCount )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CLSID )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IID )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            IModule __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parent )( 
            IModule __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyName )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyName )( 
            IModule __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IModule __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Security )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Security )( 
            IModule __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsOfType )( 
            IModule __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToAscii )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            IModule __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IModule __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            IModule __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            IModule __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallToScript )( 
            IModule __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ModuleName )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ModuleType )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plval);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Flags )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Flags )( 
            IModule __RPC_FAR * This,
            /* [in] */ long lVal);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Install )( 
            IModule __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeByName )( 
            IModule __RPC_FAR * This,
            /* [in] */ BSTR bstrSubName,
            /* [in] */ WORD wFlags,
            /* [out] */ DISPID __RPC_FAR *pdispid,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ VARIANT_BOOL bQuiet,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [out] */ HRESULT __RPC_FAR *phr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeByDispID )( 
            IModule __RPC_FAR * This,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ VARIANT_BOOL bQuiet,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [out] */ HRESULT __RPC_FAR *phr);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dispatch )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispatch);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reload )( 
            IModule __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bReinstall);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unload )( 
            IModule __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bShutdown);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Validate )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsShutdown )( 
            IModule __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        END_INTERFACE
    } IModuleVtbl;

    interface IModule
    {
        CONST_VTBL struct IModuleVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IModule_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IModule_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IModule_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IModule_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IModule_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IModule_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IModule_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IModule_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IModule_get_RefCount(This,pdwRef)	\
    (This)->lpVtbl -> get_RefCount(This,pdwRef)

#define IModule_get_CLSID(This,pclsid)	\
    (This)->lpVtbl -> get_CLSID(This,pclsid)

#define IModule_get_IID(This,piid)	\
    (This)->lpVtbl -> get_IID(This,piid)

#define IModule_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IModule_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define IModule_get_Parent(This,ppthing)	\
    (This)->lpVtbl -> get_Parent(This,ppthing)

#define IModule_put_Parent(This,pthing)	\
    (This)->lpVtbl -> put_Parent(This,pthing)

#define IModule_get_PropertyName(This,pbstr)	\
    (This)->lpVtbl -> get_PropertyName(This,pbstr)

#define IModule_put_PropertyName(This,bstr)	\
    (This)->lpVtbl -> put_PropertyName(This,bstr)

#define IModule_get_Type(This,pbstr)	\
    (This)->lpVtbl -> get_Type(This,pbstr)

#define IModule_put_Type(This,bstr)	\
    (This)->lpVtbl -> put_Type(This,bstr)

#define IModule_get_Security(This,ppSecurity)	\
    (This)->lpVtbl -> get_Security(This,ppSecurity)

#define IModule_put_Security(This,pSecurity)	\
    (This)->lpVtbl -> put_Security(This,pSecurity)

#define IModule_IsOfType(This,bstr,pbool)	\
    (This)->lpVtbl -> IsOfType(This,bstr,pbool)

#define IModule_ToAscii(This,pbstr)	\
    (This)->lpVtbl -> ToAscii(This,pbstr)

#define IModule_Dump(This)	\
    (This)->lpVtbl -> Dump(This)

#define IModule_Clone(This,pobjectproperty)	\
    (This)->lpVtbl -> Clone(This,pobjectproperty)

#define IModule_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define IModule_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define IModule_MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)


#define IModule_get_ModuleName(This,pbstr)	\
    (This)->lpVtbl -> get_ModuleName(This,pbstr)

#define IModule_get_ModuleType(This,plval)	\
    (This)->lpVtbl -> get_ModuleType(This,plval)

#define IModule_get_Flags(This,plVal)	\
    (This)->lpVtbl -> get_Flags(This,plVal)

#define IModule_put_Flags(This,lVal)	\
    (This)->lpVtbl -> put_Flags(This,lVal)

#define IModule_Install(This)	\
    (This)->lpVtbl -> Install(This)

#define IModule_InvokeByName(This,bstrSubName,wFlags,pdispid,pdispparams,bQuiet,pvar,phr)	\
    (This)->lpVtbl -> InvokeByName(This,bstrSubName,wFlags,pdispid,pdispparams,bQuiet,pvar,phr)

#define IModule_InvokeByDispID(This,dispid,wFlags,pdispparams,bQuiet,pvar,phr)	\
    (This)->lpVtbl -> InvokeByDispID(This,dispid,wFlags,pdispparams,bQuiet,pvar,phr)

#define IModule_get_Dispatch(This,ppdispatch)	\
    (This)->lpVtbl -> get_Dispatch(This,ppdispatch)

#define IModule_Reload(This,bReinstall)	\
    (This)->lpVtbl -> Reload(This,bReinstall)

#define IModule_Unload(This,bShutdown)	\
    (This)->lpVtbl -> Unload(This,bShutdown)

#define IModule_Validate(This,pboolVal)	\
    (This)->lpVtbl -> Validate(This,pboolVal)

#define IModule_get_IsShutdown(This,pboolVal)	\
    (This)->lpVtbl -> get_IsShutdown(This,pboolVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IModule_get_ModuleName_Proxy( 
    IModule __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IModule_get_ModuleName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IModule_get_ModuleType_Proxy( 
    IModule __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plval);


void __RPC_STUB IModule_get_ModuleType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IModule_get_Flags_Proxy( 
    IModule __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB IModule_get_Flags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IModule_put_Flags_Proxy( 
    IModule __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB IModule_put_Flags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IModule_Install_Proxy( 
    IModule __RPC_FAR * This);


void __RPC_STUB IModule_Install_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IModule_InvokeByName_Proxy( 
    IModule __RPC_FAR * This,
    /* [in] */ BSTR bstrSubName,
    /* [in] */ WORD wFlags,
    /* [out] */ DISPID __RPC_FAR *pdispid,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [in] */ VARIANT_BOOL bQuiet,
    /* [out] */ VARIANT __RPC_FAR *pvar,
    /* [out] */ HRESULT __RPC_FAR *phr);


void __RPC_STUB IModule_InvokeByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IModule_InvokeByDispID_Proxy( 
    IModule __RPC_FAR * This,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [in] */ VARIANT_BOOL bQuiet,
    /* [out] */ VARIANT __RPC_FAR *pvar,
    /* [out] */ HRESULT __RPC_FAR *phr);


void __RPC_STUB IModule_InvokeByDispID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IModule_get_Dispatch_Proxy( 
    IModule __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispatch);


void __RPC_STUB IModule_get_Dispatch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IModule_Reload_Proxy( 
    IModule __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bReinstall);


void __RPC_STUB IModule_Reload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IModule_Unload_Proxy( 
    IModule __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bShutdown);


void __RPC_STUB IModule_Unload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IModule_Validate_Proxy( 
    IModule __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IModule_Validate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IModule_get_IsShutdown_Proxy( 
    IModule __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IModule_get_IsShutdown_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IModule_INTERFACE_DEFINED__ */


#ifndef __ICOMModule_INTERFACE_DEFINED__
#define __ICOMModule_INTERFACE_DEFINED__

/* interface ICOMModule */
/* [unique][helpstring][dual][uuid][object] */ 

#define	DISPID_COMMODULE_MODULECLSID	( 205 )

#define	DISPID_COMMODULE_MODULEPROGID	( 206 )


EXTERN_C const IID IID_ICOMModule;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D984C-A417-11CF-9BF0-0080C7A56A8A")
    ICOMModule : public IModule
    {
    public:
        virtual /* [id][helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ModuleCLSID( 
            /* [retval][out] */ CLSID __RPC_FAR *pclsid) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_ModuleProgID( 
            /* [in] */ BSTR bstr) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICOMModuleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICOMModule __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICOMModule __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICOMModule __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            ICOMModule __RPC_FAR * This);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RefCount )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CLSID )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IID )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parent )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyName )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyName )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Security )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Security )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsOfType )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToAscii )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            ICOMModule __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallToScript )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ModuleName )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ModuleType )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plval);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Flags )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Flags )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ long lVal);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Install )( 
            ICOMModule __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeByName )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ BSTR bstrSubName,
            /* [in] */ WORD wFlags,
            /* [out] */ DISPID __RPC_FAR *pdispid,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ VARIANT_BOOL bQuiet,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [out] */ HRESULT __RPC_FAR *phr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeByDispID )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ VARIANT_BOOL bQuiet,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [out] */ HRESULT __RPC_FAR *phr);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dispatch )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispatch);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reload )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bReinstall);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unload )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bShutdown);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Validate )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsShutdown )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [id][helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ModuleCLSID )( 
            ICOMModule __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ModuleProgID )( 
            ICOMModule __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        END_INTERFACE
    } ICOMModuleVtbl;

    interface ICOMModule
    {
        CONST_VTBL struct ICOMModuleVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICOMModule_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICOMModule_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICOMModule_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICOMModule_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICOMModule_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICOMModule_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICOMModule_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICOMModule_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define ICOMModule_get_RefCount(This,pdwRef)	\
    (This)->lpVtbl -> get_RefCount(This,pdwRef)

#define ICOMModule_get_CLSID(This,pclsid)	\
    (This)->lpVtbl -> get_CLSID(This,pclsid)

#define ICOMModule_get_IID(This,piid)	\
    (This)->lpVtbl -> get_IID(This,piid)

#define ICOMModule_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define ICOMModule_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define ICOMModule_get_Parent(This,ppthing)	\
    (This)->lpVtbl -> get_Parent(This,ppthing)

#define ICOMModule_put_Parent(This,pthing)	\
    (This)->lpVtbl -> put_Parent(This,pthing)

#define ICOMModule_get_PropertyName(This,pbstr)	\
    (This)->lpVtbl -> get_PropertyName(This,pbstr)

#define ICOMModule_put_PropertyName(This,bstr)	\
    (This)->lpVtbl -> put_PropertyName(This,bstr)

#define ICOMModule_get_Type(This,pbstr)	\
    (This)->lpVtbl -> get_Type(This,pbstr)

#define ICOMModule_put_Type(This,bstr)	\
    (This)->lpVtbl -> put_Type(This,bstr)

#define ICOMModule_get_Security(This,ppSecurity)	\
    (This)->lpVtbl -> get_Security(This,ppSecurity)

#define ICOMModule_put_Security(This,pSecurity)	\
    (This)->lpVtbl -> put_Security(This,pSecurity)

#define ICOMModule_IsOfType(This,bstr,pbool)	\
    (This)->lpVtbl -> IsOfType(This,bstr,pbool)

#define ICOMModule_ToAscii(This,pbstr)	\
    (This)->lpVtbl -> ToAscii(This,pbstr)

#define ICOMModule_Dump(This)	\
    (This)->lpVtbl -> Dump(This)

#define ICOMModule_Clone(This,pobjectproperty)	\
    (This)->lpVtbl -> Clone(This,pobjectproperty)

#define ICOMModule_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define ICOMModule_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define ICOMModule_MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)


#define ICOMModule_get_ModuleName(This,pbstr)	\
    (This)->lpVtbl -> get_ModuleName(This,pbstr)

#define ICOMModule_get_ModuleType(This,plval)	\
    (This)->lpVtbl -> get_ModuleType(This,plval)

#define ICOMModule_get_Flags(This,plVal)	\
    (This)->lpVtbl -> get_Flags(This,plVal)

#define ICOMModule_put_Flags(This,lVal)	\
    (This)->lpVtbl -> put_Flags(This,lVal)

#define ICOMModule_Install(This)	\
    (This)->lpVtbl -> Install(This)

#define ICOMModule_InvokeByName(This,bstrSubName,wFlags,pdispid,pdispparams,bQuiet,pvar,phr)	\
    (This)->lpVtbl -> InvokeByName(This,bstrSubName,wFlags,pdispid,pdispparams,bQuiet,pvar,phr)

#define ICOMModule_InvokeByDispID(This,dispid,wFlags,pdispparams,bQuiet,pvar,phr)	\
    (This)->lpVtbl -> InvokeByDispID(This,dispid,wFlags,pdispparams,bQuiet,pvar,phr)

#define ICOMModule_get_Dispatch(This,ppdispatch)	\
    (This)->lpVtbl -> get_Dispatch(This,ppdispatch)

#define ICOMModule_Reload(This,bReinstall)	\
    (This)->lpVtbl -> Reload(This,bReinstall)

#define ICOMModule_Unload(This,bShutdown)	\
    (This)->lpVtbl -> Unload(This,bShutdown)

#define ICOMModule_Validate(This,pboolVal)	\
    (This)->lpVtbl -> Validate(This,pboolVal)

#define ICOMModule_get_IsShutdown(This,pboolVal)	\
    (This)->lpVtbl -> get_IsShutdown(This,pboolVal)


#define ICOMModule_get_ModuleCLSID(This,pclsid)	\
    (This)->lpVtbl -> get_ModuleCLSID(This,pclsid)

#define ICOMModule_put_ModuleProgID(This,bstr)	\
    (This)->lpVtbl -> put_ModuleProgID(This,bstr)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE ICOMModule_get_ModuleCLSID_Proxy( 
    ICOMModule __RPC_FAR * This,
    /* [retval][out] */ CLSID __RPC_FAR *pclsid);


void __RPC_STUB ICOMModule_get_ModuleCLSID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE ICOMModule_put_ModuleProgID_Proxy( 
    ICOMModule __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB ICOMModule_put_ModuleProgID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICOMModule_INTERFACE_DEFINED__ */


#ifndef __IScriptModule_INTERFACE_DEFINED__
#define __IScriptModule_INTERFACE_DEFINED__

/* interface IScriptModule */
/* [unique][helpstring][dual][uuid][object] */ 

#define	DISPID_SCRIPTMODULE_SCRIPTPATH	( 205 )


EXTERN_C const IID IID_IScriptModule;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D984E-A417-11CF-9BF0-0080C7A56A8A")
    IScriptModule : public IModule
    {
    public:
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ScriptPath( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_ScriptPath( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddScriptlet( 
            /* [in] */ BSTR bstrCode) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IScriptModuleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IScriptModule __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IScriptModule __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IScriptModule __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IScriptModule __RPC_FAR * This);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RefCount )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CLSID )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IID )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parent )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyName )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyName )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Security )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Security )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsOfType )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToAscii )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            IScriptModule __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallToScript )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ModuleName )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ModuleType )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plval);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Flags )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Flags )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ long lVal);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Install )( 
            IScriptModule __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeByName )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ BSTR bstrSubName,
            /* [in] */ WORD wFlags,
            /* [out] */ DISPID __RPC_FAR *pdispid,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ VARIANT_BOOL bQuiet,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [out] */ HRESULT __RPC_FAR *phr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeByDispID )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ VARIANT_BOOL bQuiet,
            /* [out] */ VARIANT __RPC_FAR *pvar,
            /* [out] */ HRESULT __RPC_FAR *phr);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dispatch )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispatch);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reload )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bReinstall);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unload )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bShutdown);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Validate )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsShutdown )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ScriptPath )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ScriptPath )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddScriptlet )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ BSTR bstrCode);
        
        END_INTERFACE
    } IScriptModuleVtbl;

    interface IScriptModule
    {
        CONST_VTBL struct IScriptModuleVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IScriptModule_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IScriptModule_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IScriptModule_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IScriptModule_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IScriptModule_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IScriptModule_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IScriptModule_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IScriptModule_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IScriptModule_get_RefCount(This,pdwRef)	\
    (This)->lpVtbl -> get_RefCount(This,pdwRef)

#define IScriptModule_get_CLSID(This,pclsid)	\
    (This)->lpVtbl -> get_CLSID(This,pclsid)

#define IScriptModule_get_IID(This,piid)	\
    (This)->lpVtbl -> get_IID(This,piid)

#define IScriptModule_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IScriptModule_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define IScriptModule_get_Parent(This,ppthing)	\
    (This)->lpVtbl -> get_Parent(This,ppthing)

#define IScriptModule_put_Parent(This,pthing)	\
    (This)->lpVtbl -> put_Parent(This,pthing)

#define IScriptModule_get_PropertyName(This,pbstr)	\
    (This)->lpVtbl -> get_PropertyName(This,pbstr)

#define IScriptModule_put_PropertyName(This,bstr)	\
    (This)->lpVtbl -> put_PropertyName(This,bstr)

#define IScriptModule_get_Type(This,pbstr)	\
    (This)->lpVtbl -> get_Type(This,pbstr)

#define IScriptModule_put_Type(This,bstr)	\
    (This)->lpVtbl -> put_Type(This,bstr)

#define IScriptModule_get_Security(This,ppSecurity)	\
    (This)->lpVtbl -> get_Security(This,ppSecurity)

#define IScriptModule_put_Security(This,pSecurity)	\
    (This)->lpVtbl -> put_Security(This,pSecurity)

#define IScriptModule_IsOfType(This,bstr,pbool)	\
    (This)->lpVtbl -> IsOfType(This,bstr,pbool)

#define IScriptModule_ToAscii(This,pbstr)	\
    (This)->lpVtbl -> ToAscii(This,pbstr)

#define IScriptModule_Dump(This)	\
    (This)->lpVtbl -> Dump(This)

#define IScriptModule_Clone(This,pobjectproperty)	\
    (This)->lpVtbl -> Clone(This,pobjectproperty)

#define IScriptModule_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define IScriptModule_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define IScriptModule_MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)


#define IScriptModule_get_ModuleName(This,pbstr)	\
    (This)->lpVtbl -> get_ModuleName(This,pbstr)

#define IScriptModule_get_ModuleType(This,plval)	\
    (This)->lpVtbl -> get_ModuleType(This,plval)

#define IScriptModule_get_Flags(This,plVal)	\
    (This)->lpVtbl -> get_Flags(This,plVal)

#define IScriptModule_put_Flags(This,lVal)	\
    (This)->lpVtbl -> put_Flags(This,lVal)

#define IScriptModule_Install(This)	\
    (This)->lpVtbl -> Install(This)

#define IScriptModule_InvokeByName(This,bstrSubName,wFlags,pdispid,pdispparams,bQuiet,pvar,phr)	\
    (This)->lpVtbl -> InvokeByName(This,bstrSubName,wFlags,pdispid,pdispparams,bQuiet,pvar,phr)

#define IScriptModule_InvokeByDispID(This,dispid,wFlags,pdispparams,bQuiet,pvar,phr)	\
    (This)->lpVtbl -> InvokeByDispID(This,dispid,wFlags,pdispparams,bQuiet,pvar,phr)

#define IScriptModule_get_Dispatch(This,ppdispatch)	\
    (This)->lpVtbl -> get_Dispatch(This,ppdispatch)

#define IScriptModule_Reload(This,bReinstall)	\
    (This)->lpVtbl -> Reload(This,bReinstall)

#define IScriptModule_Unload(This,bShutdown)	\
    (This)->lpVtbl -> Unload(This,bShutdown)

#define IScriptModule_Validate(This,pboolVal)	\
    (This)->lpVtbl -> Validate(This,pboolVal)

#define IScriptModule_get_IsShutdown(This,pboolVal)	\
    (This)->lpVtbl -> get_IsShutdown(This,pboolVal)


#define IScriptModule_get_ScriptPath(This,pbstr)	\
    (This)->lpVtbl -> get_ScriptPath(This,pbstr)

#define IScriptModule_put_ScriptPath(This,bstr)	\
    (This)->lpVtbl -> put_ScriptPath(This,bstr)

#define IScriptModule_AddScriptlet(This,bstrCode)	\
    (This)->lpVtbl -> AddScriptlet(This,bstrCode)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IScriptModule_get_ScriptPath_Proxy( 
    IScriptModule __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IScriptModule_get_ScriptPath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IScriptModule_put_ScriptPath_Proxy( 
    IScriptModule __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IScriptModule_put_ScriptPath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IScriptModule_AddScriptlet_Proxy( 
    IScriptModule __RPC_FAR * This,
    /* [in] */ BSTR bstrCode);


void __RPC_STUB IScriptModule_AddScriptlet_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IScriptModule_INTERFACE_DEFINED__ */


#ifndef __IMethod_INTERFACE_DEFINED__
#define __IMethod_INTERFACE_DEFINED__

/* interface IMethod */
/* [unique][helpstring][dual][uuid][object] */ 

#define	DISPID_METHOD_FLAGS	( 101 )

#define	DISPID_METHOD_MODULE	( 102 )

#define	DISPID_METHOD_DISPID	( 103 )

#define	DISPID_METHOD_METHODNAME	( 104 )

#define	DISPID_METHOD_ARGUMENTSINT	( 105 )

#define	DISPID_METHOD_CODE	( 106 )


EXTERN_C const IID IID_IMethod;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D9850-A417-11CF-9BF0-0080C7A56A8A")
    IMethod : public IObjectProperty
    {
    public:
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Flags( 
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_Flags( 
            /* [in] */ long lVal) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Module( 
            /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_Module( 
            /* [in] */ IModule __RPC_FAR *pmodule) = 0;
        
        virtual /* [id][helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_DispID( 
            /* [retval][out] */ DISPID __RPC_FAR *pdispid) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_DispID( 
            /* [in] */ DISPID dispid) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_MethodName( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_MethodName( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Code( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_Code( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_IsInline( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE InvokeMethodExt( 
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [out] */ HRESULT __RPC_FAR *phr,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_ArgumentsInt( 
            /* [in] */ IPropertyList __RPC_FAR *pproplist) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_ArgumentsExt( 
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams) = 0;
        
        virtual /* [helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_Arguments( 
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Validate( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMethodVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMethod __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMethod __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMethod __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMethod __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMethod __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMethod __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMethod __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IMethod __RPC_FAR * This);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RefCount )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CLSID )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IID )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            IMethod __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parent )( 
            IMethod __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyName )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyName )( 
            IMethod __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IMethod __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Security )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Security )( 
            IMethod __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsOfType )( 
            IMethod __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToAscii )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            IMethod __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IMethod __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            IMethod __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            IMethod __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallToScript )( 
            IMethod __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Flags )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Flags )( 
            IMethod __RPC_FAR * This,
            /* [in] */ long lVal);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Module )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Module )( 
            IMethod __RPC_FAR * This,
            /* [in] */ IModule __RPC_FAR *pmodule);
        
        /* [id][helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DispID )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ DISPID __RPC_FAR *pdispid);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DispID )( 
            IMethod __RPC_FAR * This,
            /* [in] */ DISPID dispid);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MethodName )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MethodName )( 
            IMethod __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            IMethod __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsInline )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeMethodExt )( 
            IMethod __RPC_FAR * This,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [out] */ HRESULT __RPC_FAR *phr,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ArgumentsInt )( 
            IMethod __RPC_FAR * This,
            /* [in] */ IPropertyList __RPC_FAR *pproplist);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ArgumentsExt )( 
            IMethod __RPC_FAR * This,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);
        
        /* [helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Arguments )( 
            IMethod __RPC_FAR * This,
            /* [in] */ VARIANT var);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Validate )( 
            IMethod __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        END_INTERFACE
    } IMethodVtbl;

    interface IMethod
    {
        CONST_VTBL struct IMethodVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMethod_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMethod_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMethod_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMethod_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMethod_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMethod_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMethod_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMethod_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IMethod_get_RefCount(This,pdwRef)	\
    (This)->lpVtbl -> get_RefCount(This,pdwRef)

#define IMethod_get_CLSID(This,pclsid)	\
    (This)->lpVtbl -> get_CLSID(This,pclsid)

#define IMethod_get_IID(This,piid)	\
    (This)->lpVtbl -> get_IID(This,piid)

#define IMethod_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IMethod_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define IMethod_get_Parent(This,ppthing)	\
    (This)->lpVtbl -> get_Parent(This,ppthing)

#define IMethod_put_Parent(This,pthing)	\
    (This)->lpVtbl -> put_Parent(This,pthing)

#define IMethod_get_PropertyName(This,pbstr)	\
    (This)->lpVtbl -> get_PropertyName(This,pbstr)

#define IMethod_put_PropertyName(This,bstr)	\
    (This)->lpVtbl -> put_PropertyName(This,bstr)

#define IMethod_get_Type(This,pbstr)	\
    (This)->lpVtbl -> get_Type(This,pbstr)

#define IMethod_put_Type(This,bstr)	\
    (This)->lpVtbl -> put_Type(This,bstr)

#define IMethod_get_Security(This,ppSecurity)	\
    (This)->lpVtbl -> get_Security(This,ppSecurity)

#define IMethod_put_Security(This,pSecurity)	\
    (This)->lpVtbl -> put_Security(This,pSecurity)

#define IMethod_IsOfType(This,bstr,pbool)	\
    (This)->lpVtbl -> IsOfType(This,bstr,pbool)

#define IMethod_ToAscii(This,pbstr)	\
    (This)->lpVtbl -> ToAscii(This,pbstr)

#define IMethod_Dump(This)	\
    (This)->lpVtbl -> Dump(This)

#define IMethod_Clone(This,pobjectproperty)	\
    (This)->lpVtbl -> Clone(This,pobjectproperty)

#define IMethod_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define IMethod_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define IMethod_MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)


#define IMethod_get_Flags(This,plVal)	\
    (This)->lpVtbl -> get_Flags(This,plVal)

#define IMethod_put_Flags(This,lVal)	\
    (This)->lpVtbl -> put_Flags(This,lVal)

#define IMethod_get_Module(This,ppmodule)	\
    (This)->lpVtbl -> get_Module(This,ppmodule)

#define IMethod_put_Module(This,pmodule)	\
    (This)->lpVtbl -> put_Module(This,pmodule)

#define IMethod_get_DispID(This,pdispid)	\
    (This)->lpVtbl -> get_DispID(This,pdispid)

#define IMethod_put_DispID(This,dispid)	\
    (This)->lpVtbl -> put_DispID(This,dispid)

#define IMethod_get_MethodName(This,pbstr)	\
    (This)->lpVtbl -> get_MethodName(This,pbstr)

#define IMethod_put_MethodName(This,bstr)	\
    (This)->lpVtbl -> put_MethodName(This,bstr)

#define IMethod_get_Code(This,pbstr)	\
    (This)->lpVtbl -> get_Code(This,pbstr)

#define IMethod_put_Code(This,bstr)	\
    (This)->lpVtbl -> put_Code(This,bstr)

#define IMethod_get_IsInline(This,pbool)	\
    (This)->lpVtbl -> get_IsInline(This,pbool)

#define IMethod_InvokeMethodExt(This,pdispparams,phr,pvar)	\
    (This)->lpVtbl -> InvokeMethodExt(This,pdispparams,phr,pvar)

#define IMethod_put_ArgumentsInt(This,pproplist)	\
    (This)->lpVtbl -> put_ArgumentsInt(This,pproplist)

#define IMethod_put_ArgumentsExt(This,pdispparams)	\
    (This)->lpVtbl -> put_ArgumentsExt(This,pdispparams)

#define IMethod_put_Arguments(This,var)	\
    (This)->lpVtbl -> put_Arguments(This,var)

#define IMethod_Validate(This,pboolVal)	\
    (This)->lpVtbl -> Validate(This,pboolVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMethod_get_Flags_Proxy( 
    IMethod __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB IMethod_get_Flags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IMethod_put_Flags_Proxy( 
    IMethod __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB IMethod_put_Flags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMethod_get_Module_Proxy( 
    IMethod __RPC_FAR * This,
    /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule);


void __RPC_STUB IMethod_get_Module_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IMethod_put_Module_Proxy( 
    IMethod __RPC_FAR * This,
    /* [in] */ IModule __RPC_FAR *pmodule);


void __RPC_STUB IMethod_put_Module_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IMethod_get_DispID_Proxy( 
    IMethod __RPC_FAR * This,
    /* [retval][out] */ DISPID __RPC_FAR *pdispid);


void __RPC_STUB IMethod_get_DispID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IMethod_put_DispID_Proxy( 
    IMethod __RPC_FAR * This,
    /* [in] */ DISPID dispid);


void __RPC_STUB IMethod_put_DispID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMethod_get_MethodName_Proxy( 
    IMethod __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IMethod_get_MethodName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IMethod_put_MethodName_Proxy( 
    IMethod __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IMethod_put_MethodName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMethod_get_Code_Proxy( 
    IMethod __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IMethod_get_Code_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IMethod_put_Code_Proxy( 
    IMethod __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IMethod_put_Code_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IMethod_get_IsInline_Proxy( 
    IMethod __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IMethod_get_IsInline_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IMethod_InvokeMethodExt_Proxy( 
    IMethod __RPC_FAR * This,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [out] */ HRESULT __RPC_FAR *phr,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IMethod_InvokeMethodExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IMethod_put_ArgumentsInt_Proxy( 
    IMethod __RPC_FAR * This,
    /* [in] */ IPropertyList __RPC_FAR *pproplist);


void __RPC_STUB IMethod_put_ArgumentsInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMethod_put_ArgumentsExt_Proxy( 
    IMethod __RPC_FAR * This,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);


void __RPC_STUB IMethod_put_ArgumentsExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IMethod_put_Arguments_Proxy( 
    IMethod __RPC_FAR * This,
    /* [in] */ VARIANT var);


void __RPC_STUB IMethod_put_Arguments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IMethod_Validate_Proxy( 
    IMethod __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IMethod_Validate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMethod_INTERFACE_DEFINED__ */


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
