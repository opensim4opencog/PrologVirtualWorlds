/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:33 2000
 */
/* Compiler settings for menuitem.idl:
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

#ifndef __menuitem_h__
#define __menuitem_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IMenuItem_FWD_DEFINED__
#define __IMenuItem_FWD_DEFINED__
typedef interface IMenuItem IMenuItem;
#endif 	/* __IMenuItem_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "vwprop.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IMenuItem_INTERFACE_DEFINED__
#define __IMenuItem_INTERFACE_DEFINED__

/* interface IMenuItem */
/* [unique][helpstring][local][dual][uuid][object] */ 

#define	DISPID_MENUITEM_MENUTEXT	( 101 )

#define	DISPID_MENUITEM_FLAGS	( 102 )

#define	DISPID_MENUITEM_USERDATA	( 103 )

#define	DISPID_MENUITEM_STATUSTEXT	( 104 )

#define	DISPID_MENUITEM_METHOD	( 105 )

#define	DISPID_MENUITEM_ONUPDATEMETHOD	( 106 )

#define	DISPID_MENUITEM_ARGUMENTSINT	( 107 )

#define	DISPID_MENUITEM_OWNER	( 108 )

#define	DISPID_MENUITEM_SUBMENU	( 109 )

#define	DISPID_MENUITEM_TARGET	( 110 )

#define	DISPID_MENUITEM_ADDTARGETPARAMETER	( 111 )

#define	DISPID_MENUITEM_ISOVERLAYITEM	( 112 )

#define	DISPID_MENUITEM_ISPROXIMITYSENSITIVE	( 113 )

#define	MI_DEFAULT	( 0 )

#define	MI_GRAYED	( 0x1 )

#define	MI_DISABLED	( 0x2 )

#define	MI_INVISIBLE	( 0x4 )

#define	MI_CHECKED	( 0x8 )

#define	MI_POPUP	( 0x10 )

#define	MI_SEPARATOR	( 0x800 )

#define	MI_USEVERBARGS	( 0x80000000 )


EXTERN_C const IID IID_IMenuItem;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D4916C22-00C2-11D0-8A96-00A0C9082583")
    IMenuItem : public IObjectProperty
    {
    public:
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_MenuText( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrMenuText) = 0;
        
        virtual /* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_MenuText( 
            /* [in] */ BSTR bstrMenuText) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Flags( 
            /* [retval][out] */ long __RPC_FAR *nFlags) = 0;
        
        virtual /* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Flags( 
            /* [in] */ long nFlags) = 0;
        
        virtual /* [id][helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_UserData( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_UserData( 
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_SubMenu( 
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pvar) = 0;
        
        virtual /* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_SubMenu( 
            /* [in] */ IPropertyList __RPC_FAR *var) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_StatusText( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrStatusText) = 0;
        
        virtual /* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_StatusText( 
            /* [in] */ BSTR bstrStatusText) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Method( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrMethod) = 0;
        
        virtual /* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Method( 
            /* [in] */ BSTR bstrMethod) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_OnUpdateMethod( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOnUpdateMethod) = 0;
        
        virtual /* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_OnUpdateMethod( 
            /* [in] */ BSTR bstrOnUpdateMethod) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_ArgumentsInt( 
            /* [in] */ IPropertyList __RPC_FAR *pproplist) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ArgumentsExt( 
            /* [retval][out] */ DISPPARAMS __RPC_FAR *__RPC_FAR *ppdispparams) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_ArgumentsExt( 
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Arguments( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Arguments( 
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Owner( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppIThing) = 0;
        
        virtual /* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Owner( 
            /* [in] */ IThing __RPC_FAR *pIThing) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Target( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppIThing) = 0;
        
        virtual /* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Target( 
            /* [in] */ IThing __RPC_FAR *pIThing) = 0;
        
        virtual /* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_AddTargetParameter( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolAddTargetParameter) = 0;
        
        virtual /* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_AddTargetParameter( 
            /* [in] */ VARIANT_BOOL boolAddTargetParameter) = 0;
        
        virtual /* [id][helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_IsOverlayItem( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolIsOverlayItem) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_IsOverlayItem( 
            /* [in] */ VARIANT_BOOL boolIsOverlayItem) = 0;
        
        virtual /* [id][helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_IsProximitySensitive( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolIsProximitySensitive) = 0;
        
        virtual /* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_IsProximitySensitive( 
            /* [in] */ VARIANT_BOOL boolIsProximitySensitive) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Execute( void) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE OnUpdate( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Duplicate( 
            /* [retval][out] */ IMenuItem __RPC_FAR *__RPC_FAR *ppIMenuItem) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMenuItemVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMenuItem __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMenuItem __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMenuItem __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IMenuItem __RPC_FAR * This);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RefCount )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CLSID )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IID )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parent )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyName )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyName )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Security )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Security )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsOfType )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToAscii )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            IMenuItem __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallToScript )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MenuText )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrMenuText);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MenuText )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ BSTR bstrMenuText);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Flags )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *nFlags);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Flags )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ long nFlags);
        
        /* [id][helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UserData )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UserData )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ VARIANT var);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SubMenu )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pvar);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SubMenu )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ IPropertyList __RPC_FAR *var);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StatusText )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrStatusText);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_StatusText )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ BSTR bstrStatusText);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Method )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrMethod);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Method )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ BSTR bstrMethod);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_OnUpdateMethod )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOnUpdateMethod);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_OnUpdateMethod )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ BSTR bstrOnUpdateMethod);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ArgumentsInt )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ IPropertyList __RPC_FAR *pproplist);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ArgumentsExt )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ DISPPARAMS __RPC_FAR *__RPC_FAR *ppdispparams);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ArgumentsExt )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Arguments )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvar);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Arguments )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ VARIANT var);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Owner )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppIThing);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Owner )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pIThing);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Target )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppIThing);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Target )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pIThing);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AddTargetParameter )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolAddTargetParameter);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AddTargetParameter )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL boolAddTargetParameter);
        
        /* [id][helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsOverlayItem )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolIsOverlayItem);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_IsOverlayItem )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL boolIsOverlayItem);
        
        /* [id][helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsProximitySensitive )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolIsProximitySensitive);
        
        /* [id][helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_IsProximitySensitive )( 
            IMenuItem __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL boolIsProximitySensitive);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Execute )( 
            IMenuItem __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUpdate )( 
            IMenuItem __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Duplicate )( 
            IMenuItem __RPC_FAR * This,
            /* [retval][out] */ IMenuItem __RPC_FAR *__RPC_FAR *ppIMenuItem);
        
        END_INTERFACE
    } IMenuItemVtbl;

    interface IMenuItem
    {
        CONST_VTBL struct IMenuItemVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMenuItem_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMenuItem_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMenuItem_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMenuItem_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMenuItem_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMenuItem_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMenuItem_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMenuItem_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IMenuItem_get_RefCount(This,pdwRef)	\
    (This)->lpVtbl -> get_RefCount(This,pdwRef)

#define IMenuItem_get_CLSID(This,pclsid)	\
    (This)->lpVtbl -> get_CLSID(This,pclsid)

#define IMenuItem_get_IID(This,piid)	\
    (This)->lpVtbl -> get_IID(This,piid)

#define IMenuItem_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IMenuItem_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define IMenuItem_get_Parent(This,ppthing)	\
    (This)->lpVtbl -> get_Parent(This,ppthing)

#define IMenuItem_put_Parent(This,pthing)	\
    (This)->lpVtbl -> put_Parent(This,pthing)

#define IMenuItem_get_PropertyName(This,pbstr)	\
    (This)->lpVtbl -> get_PropertyName(This,pbstr)

#define IMenuItem_put_PropertyName(This,bstr)	\
    (This)->lpVtbl -> put_PropertyName(This,bstr)

#define IMenuItem_get_Type(This,pbstr)	\
    (This)->lpVtbl -> get_Type(This,pbstr)

#define IMenuItem_put_Type(This,bstr)	\
    (This)->lpVtbl -> put_Type(This,bstr)

#define IMenuItem_get_Security(This,ppSecurity)	\
    (This)->lpVtbl -> get_Security(This,ppSecurity)

#define IMenuItem_put_Security(This,pSecurity)	\
    (This)->lpVtbl -> put_Security(This,pSecurity)

#define IMenuItem_IsOfType(This,bstr,pbool)	\
    (This)->lpVtbl -> IsOfType(This,bstr,pbool)

#define IMenuItem_ToAscii(This,pbstr)	\
    (This)->lpVtbl -> ToAscii(This,pbstr)

#define IMenuItem_Dump(This)	\
    (This)->lpVtbl -> Dump(This)

#define IMenuItem_Clone(This,pobjectproperty)	\
    (This)->lpVtbl -> Clone(This,pobjectproperty)

#define IMenuItem_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define IMenuItem_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define IMenuItem_MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)


#define IMenuItem_get_MenuText(This,pbstrMenuText)	\
    (This)->lpVtbl -> get_MenuText(This,pbstrMenuText)

#define IMenuItem_put_MenuText(This,bstrMenuText)	\
    (This)->lpVtbl -> put_MenuText(This,bstrMenuText)

#define IMenuItem_get_Flags(This,nFlags)	\
    (This)->lpVtbl -> get_Flags(This,nFlags)

#define IMenuItem_put_Flags(This,nFlags)	\
    (This)->lpVtbl -> put_Flags(This,nFlags)

#define IMenuItem_get_UserData(This,pvar)	\
    (This)->lpVtbl -> get_UserData(This,pvar)

#define IMenuItem_put_UserData(This,var)	\
    (This)->lpVtbl -> put_UserData(This,var)

#define IMenuItem_get_SubMenu(This,pvar)	\
    (This)->lpVtbl -> get_SubMenu(This,pvar)

#define IMenuItem_put_SubMenu(This,var)	\
    (This)->lpVtbl -> put_SubMenu(This,var)

#define IMenuItem_get_StatusText(This,pbstrStatusText)	\
    (This)->lpVtbl -> get_StatusText(This,pbstrStatusText)

#define IMenuItem_put_StatusText(This,bstrStatusText)	\
    (This)->lpVtbl -> put_StatusText(This,bstrStatusText)

#define IMenuItem_get_Method(This,pbstrMethod)	\
    (This)->lpVtbl -> get_Method(This,pbstrMethod)

#define IMenuItem_put_Method(This,bstrMethod)	\
    (This)->lpVtbl -> put_Method(This,bstrMethod)

#define IMenuItem_get_OnUpdateMethod(This,pbstrOnUpdateMethod)	\
    (This)->lpVtbl -> get_OnUpdateMethod(This,pbstrOnUpdateMethod)

#define IMenuItem_put_OnUpdateMethod(This,bstrOnUpdateMethod)	\
    (This)->lpVtbl -> put_OnUpdateMethod(This,bstrOnUpdateMethod)

#define IMenuItem_put_ArgumentsInt(This,pproplist)	\
    (This)->lpVtbl -> put_ArgumentsInt(This,pproplist)

#define IMenuItem_get_ArgumentsExt(This,ppdispparams)	\
    (This)->lpVtbl -> get_ArgumentsExt(This,ppdispparams)

#define IMenuItem_put_ArgumentsExt(This,pdispparams)	\
    (This)->lpVtbl -> put_ArgumentsExt(This,pdispparams)

#define IMenuItem_get_Arguments(This,pvar)	\
    (This)->lpVtbl -> get_Arguments(This,pvar)

#define IMenuItem_put_Arguments(This,var)	\
    (This)->lpVtbl -> put_Arguments(This,var)

#define IMenuItem_get_Owner(This,ppIThing)	\
    (This)->lpVtbl -> get_Owner(This,ppIThing)

#define IMenuItem_put_Owner(This,pIThing)	\
    (This)->lpVtbl -> put_Owner(This,pIThing)

#define IMenuItem_get_Target(This,ppIThing)	\
    (This)->lpVtbl -> get_Target(This,ppIThing)

#define IMenuItem_put_Target(This,pIThing)	\
    (This)->lpVtbl -> put_Target(This,pIThing)

#define IMenuItem_get_AddTargetParameter(This,pboolAddTargetParameter)	\
    (This)->lpVtbl -> get_AddTargetParameter(This,pboolAddTargetParameter)

#define IMenuItem_put_AddTargetParameter(This,boolAddTargetParameter)	\
    (This)->lpVtbl -> put_AddTargetParameter(This,boolAddTargetParameter)

#define IMenuItem_get_IsOverlayItem(This,pboolIsOverlayItem)	\
    (This)->lpVtbl -> get_IsOverlayItem(This,pboolIsOverlayItem)

#define IMenuItem_put_IsOverlayItem(This,boolIsOverlayItem)	\
    (This)->lpVtbl -> put_IsOverlayItem(This,boolIsOverlayItem)

#define IMenuItem_get_IsProximitySensitive(This,pboolIsProximitySensitive)	\
    (This)->lpVtbl -> get_IsProximitySensitive(This,pboolIsProximitySensitive)

#define IMenuItem_put_IsProximitySensitive(This,boolIsProximitySensitive)	\
    (This)->lpVtbl -> put_IsProximitySensitive(This,boolIsProximitySensitive)

#define IMenuItem_Execute(This)	\
    (This)->lpVtbl -> Execute(This)

#define IMenuItem_OnUpdate(This)	\
    (This)->lpVtbl -> OnUpdate(This)

#define IMenuItem_Duplicate(This,ppIMenuItem)	\
    (This)->lpVtbl -> Duplicate(This,ppIMenuItem)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMenuItem_get_MenuText_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrMenuText);


void __RPC_STUB IMenuItem_get_MenuText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_MenuText_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ BSTR bstrMenuText);


void __RPC_STUB IMenuItem_put_MenuText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMenuItem_get_Flags_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *nFlags);


void __RPC_STUB IMenuItem_get_Flags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_Flags_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ long nFlags);


void __RPC_STUB IMenuItem_put_Flags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IMenuItem_get_UserData_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IMenuItem_get_UserData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_UserData_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ VARIANT var);


void __RPC_STUB IMenuItem_put_UserData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMenuItem_get_SubMenu_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pvar);


void __RPC_STUB IMenuItem_get_SubMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_SubMenu_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ IPropertyList __RPC_FAR *var);


void __RPC_STUB IMenuItem_put_SubMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMenuItem_get_StatusText_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrStatusText);


void __RPC_STUB IMenuItem_get_StatusText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_StatusText_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ BSTR bstrStatusText);


void __RPC_STUB IMenuItem_put_StatusText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMenuItem_get_Method_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrMethod);


void __RPC_STUB IMenuItem_get_Method_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_Method_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ BSTR bstrMethod);


void __RPC_STUB IMenuItem_put_Method_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMenuItem_get_OnUpdateMethod_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrOnUpdateMethod);


void __RPC_STUB IMenuItem_get_OnUpdateMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_OnUpdateMethod_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ BSTR bstrOnUpdateMethod);


void __RPC_STUB IMenuItem_put_OnUpdateMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_ArgumentsInt_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ IPropertyList __RPC_FAR *pproplist);


void __RPC_STUB IMenuItem_put_ArgumentsInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IMenuItem_get_ArgumentsExt_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ DISPPARAMS __RPC_FAR *__RPC_FAR *ppdispparams);


void __RPC_STUB IMenuItem_get_ArgumentsExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_ArgumentsExt_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);


void __RPC_STUB IMenuItem_put_ArgumentsExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMenuItem_get_Arguments_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IMenuItem_get_Arguments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_Arguments_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ VARIANT var);


void __RPC_STUB IMenuItem_put_Arguments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMenuItem_get_Owner_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppIThing);


void __RPC_STUB IMenuItem_get_Owner_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_Owner_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pIThing);


void __RPC_STUB IMenuItem_put_Owner_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMenuItem_get_Target_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppIThing);


void __RPC_STUB IMenuItem_get_Target_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_Target_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pIThing);


void __RPC_STUB IMenuItem_put_Target_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMenuItem_get_AddTargetParameter_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolAddTargetParameter);


void __RPC_STUB IMenuItem_get_AddTargetParameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_AddTargetParameter_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL boolAddTargetParameter);


void __RPC_STUB IMenuItem_put_AddTargetParameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IMenuItem_get_IsOverlayItem_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolIsOverlayItem);


void __RPC_STUB IMenuItem_get_IsOverlayItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_IsOverlayItem_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL boolIsOverlayItem);


void __RPC_STUB IMenuItem_put_IsOverlayItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IMenuItem_get_IsProximitySensitive_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolIsProximitySensitive);


void __RPC_STUB IMenuItem_get_IsProximitySensitive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IMenuItem_put_IsProximitySensitive_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL boolIsProximitySensitive);


void __RPC_STUB IMenuItem_put_IsProximitySensitive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMenuItem_Execute_Proxy( 
    IMenuItem __RPC_FAR * This);


void __RPC_STUB IMenuItem_Execute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IMenuItem_OnUpdate_Proxy( 
    IMenuItem __RPC_FAR * This);


void __RPC_STUB IMenuItem_OnUpdate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMenuItem_Duplicate_Proxy( 
    IMenuItem __RPC_FAR * This,
    /* [retval][out] */ IMenuItem __RPC_FAR *__RPC_FAR *ppIMenuItem);


void __RPC_STUB IMenuItem_Duplicate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMenuItem_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
