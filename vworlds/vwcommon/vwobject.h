/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:31 2000
 */
/* Compiler settings for vwobject.idl:
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

#ifndef __vwobject_h__
#define __vwobject_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IWorld_FWD_DEFINED__
#define __IWorld_FWD_DEFINED__
typedef interface IWorld IWorld;
#endif 	/* __IWorld_FWD_DEFINED__ */


#ifndef __IVWObjectSite_FWD_DEFINED__
#define __IVWObjectSite_FWD_DEFINED__
typedef interface IVWObjectSite IVWObjectSite;
#endif 	/* __IVWObjectSite_FWD_DEFINED__ */


#ifndef __IVWDispatch_FWD_DEFINED__
#define __IVWDispatch_FWD_DEFINED__
typedef interface IVWDispatch IVWDispatch;
#endif 	/* __IVWDispatch_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "vwcomm.h"
#include "vwprop.h"
#include "avprof.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_vwobject_0000 */
/* [local] */ 









#define NULL_OBJECT						0
#define WORLD_OBJECT						1
#define MARSHALL_BYVALUE						0x00000001
#define MARSHALL_BYREF						0x00000002
#define MARSHALL_BYGUID						0x00000010
#define MARSHALL_BYID						0x00000020
#define MARSHALL_TODISK						0x00000100
#define MARSHALL_TONETWORK					0x00000200
#define MARSHALL_TOMEMORY					0x00000400
#define MARSHALL_BYVALUEMASK					0xFFFFFFF0
#define MARSHALL_IDMASK						0xFFFFFF0F
#define MARSHALL_LOCATIONMASK				0xFFFFF0FF
#define MARSHALLDEST(a,b) ((a & MARSHALL_IDMASK) | b)
#define MARSHALLREF(a,b) ((a & MARSHALL_BYVALUEMASK) | b)
#define MARSHALLLOCATION(a,b) ((a & MARSHALL_LOCATIONMASK) | b)
#define EXTRACTREF(a) (a & ~(MARSHALL_BYVALUEMASK))
#define EXTRACTID(a) (a & ~(MARSHALL_IDMASK))
#define EXTRACTLOCATION(a) (a & ~(MARSHALL_LOCATIONMASK))
#define METHOD_CLIENT						0x00000001
#define METHOD_SERVER						0x00000002
#define METHOD_SYNC							0x00000010
#define METHOD_INLINE						0x00000100
#define METHOD_PROPERTYPUT					0x00001000
#define METHOD_PROPERTYGET					0x00002000
#define METHOD_CLIENTSERVERMASK				0xFFFFFFF0
#define METHOD_SYNCMASK						0xFFFFFF0F
#define METHOD_INLINEMASK					0xFFFFF0FF
#define METHOD_INVOKEMASK					0xFFFF0FFF
#define METHODCLIENTSERVER(a,b) ((a & METHOD_CLIENTSERVERMASK) | b)
#define METHODSYNC(a,b) ((a & METHOD_SYNCMASK) | b)
#define METHODINLINE(a,b) ((a & METHOD_INLINEMASK) | b)
#define METHODINVOKE(a,b) ((a & METHOD_INVOKEMASK) | b)
#define MODULE_CLIENT						0x00000001
#define MODULE_SERVER						0x00000002
#define MODULE_LOCAL							0x00000010
#define MODULE_CLIENTSERVERMASK				0xFFFFFFF0
#define MODULE_LOCALMASK						0xFFFFFF0F
#define MODULECLIENTSERVER(a,b) ((a & MODULE_CLIENTSERVERMASK) | b)
#define MODULELOCAL(a,b) ((a & MODULE_LOCALMASK) | b)
#define VWOBJECT_S_NOTREMOTED			MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x0201)
#define VWOBJECT_S_NOTINSTANCED			MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x0202)
#define VWOBJECT_S_ALREADYDISCONNECTED	MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x0203)
#define VWOBJECT_E_INVOKEEXCEPTION		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0201)
#define VWOBJECT_E_FILEEXCEPTION			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0202)
#define VWOBJECT_E_FILEREADERROR			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0203)
#define VWOBJECT_E_INVALIDMODULE			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0204)
#define VWOBJECT_E_COLLECTIONERROR		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0205)
#define VWOBJECT_E_WRONGPROPERTYTYPE		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0206)
#define VWOBJECT_E_UNKNOWNTYPE			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0207)
#define VWOBJECT_E_INVALIDPROPERTYLIST	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0208)
#define VWOBJECT_E_PROPERTYEXISTS		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0209)
#define VWOBJECT_E_INVALIDPROPERTYMAP	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x020A)
#define VWOBJECT_E_PROPERTYNOTEXIST		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x020C)
#define VWOBJECT_E_METHODEXISTS			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x020D)
#define VWOBJECT_E_METHODNOTEXIST		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x020F)
#define VWOBJECT_E_INVALIDMETHOD			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0210)
#define VWOBJECT_E_INVALIDMETHODMAP		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0211)
#define VWOBJECT_E_CONNECTIONERROR		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0212)
#define VWOBJECT_E_INVALIDCREATUREMAP	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0213)
#define VWOBJECT_E_EXEMPLAREXISTS		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0214)
#define VWOBJECT_E_INVALIDOBJECT			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0215)
#define VWOBJECT_E_EXEMPLARNOTEXIST		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0216)
#define VWOBJECT_E_INVALIDEXEMPLARMAP	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0217)
#define VWOBJECT_E_CREATUREEXISTS		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0218)
#define VWOBJECT_E_INVALIDTOOLMAP		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0219)
#define VWOBJECT_E_INVALIDMODULEMAP		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x021A)
#define VWOBJECT_E_STUBOBJECT			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x021B)
#define VWOBJECT_E_UNKNOWNPROPERTYTYPE	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x021C)
#define VWOBJECT_E_UNKNOWNMESSAGETYPE	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x021D)
#define VWOBJECT_E_MODULEEXISTS			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x021E)
#define VWOBJECT_E_MODULENOTEXIST		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x021F)
#define VWOBJECT_E_TOOLEXISTS			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0220)
#define VWOBJECT_E_TOOLNOTEXIST			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0221)
#define VWOBJECT_E_INVALIDMODULETYPE		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0222)
#define VWOBJECT_E_UNKNOWNPROGID			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0223)
#define VWOBJECT_E_INVALIDCONNECTION		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0224)
#define VWOBJECT_E_INTERFACENOTSUPPORTED	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0225)
#define VWOBJECT_E_INVALIDTRACELEVEL		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0226)
#define VWOBJECT_E_COLLECTIONLEVEL		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0227)
#define VWOBJECT_E_PERMISSION_DENIED		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0228)
#define VWOBJECT_E_SYNTAX_ERROR			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0229)
#define VWOBJECT_E_UNKNOWNTRACECOMPONENT	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x022A)
#define VWOBJECT_E_ALREADYCONNECTED		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x022B)
#define VWOBJECT_E_OBJECTNOTFOUND		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x022C)
#define VWOBJECT_E_UNKNOWNCLSIDINDEX		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x022D)
#define VWOBJECT_E_INVALIDLISTINDEX		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x022E)
#define VWOBJECT_E_EMPTYPROPERTYLIST		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x022F)
#define VWOBJECT_E_TYPE_MISMATCH			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0230)
#define VWOBJECT_E_INVALIDDEFAULTSCRIPT	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0231)
#define VWOBJECT_E_INVALIDWORLD			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0232)
#define VWOBJECT_E_INVALIDBITSTATE		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0233)
#define VWOBJECT_E_EVENTABORTED			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0234)
#define VWOBJECT_E_EXEMPLARNOTFERTILE	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0235)
#define VWOBJECT_E_INVALIDPROPERTYNAME	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0236)
#define VWOBJECT_E_INVALIDEXEMPLARTYPE	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0237)
#define VWOBJECT_E_INVALIDINSTANCENAME	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0238)
#define VWOBJECT_E_INVALIDSCRIPTENGINE	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0239)
#define VWOBJECT_E_UNKNOWNSCRIPTERROR	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x023a)
#define VWOBJECT_E_INVALIDCONTAINER		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x023b)
#define VWOBJECT_E_AVATARNOTEXIST		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x023c)
#define VWOBJECT_E_INVALIDSECURITY		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x023d)
#define VWOBJECT_E_ONLYRECONNECTFROMCLIENT		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x023e)
#define VWOBJECT_E_OBJECTNOTEXEMPLAR		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x023f)
#define VWOBJECT_E_INSTANCENOTCLONEABLE	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0240)
#define VWOBJECT_E_EXEMPLARNOTCLONEABLE	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0241)
#define VWOBJECT_E_INVALIDURLFORMAT		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0242)
#define VWOBJECT_E_INVALIDDESTINATION	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0243)
#define VWOBJECT_E_ONLYSERVERSIDE		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0244)
#define VWOBJECT_E_INVALIDPARENT			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0245)
#define VWOBJECT_E_INVALIDWORLDREGISTRY	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0246)
#define VWOBJECT_E_PROCESSMSGEXCEPTION	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0247)
#define VWOBJECT_E_INVALIDBSTRSIZE		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0248)
#define VWOBJECT_E_ALREADYDESTROYED		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0249)
#define VWOBJECT_E_INVALIDEXEMPLAR		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x024a)
#define VWOBJECT_E_INVALIDPASSWORD		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x024b)
#define VWOBJECT_E_CANTDESTROYUSER		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x024c)
#define VWOBJECT_E_CANTDESTROYCONTAINER	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x024d)
#define VWOBJECT_E_CANTDESTROYROOM		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x024e)
#define VWOBJECT_E_INVALIDCONTENTPATH	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x024f)
#define VWOBJECT_E_INVALIDAVATARGRAPHICSPATH	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0250)
#define VWOBJECT_E_INVALIDAVATARPATH		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0251)
#define VWOBJECT_E_INVALIDPROFILE		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0252)
#define VWOBJECT_E_INVALIDHELPPATH		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0253)
#define VWOBJECT_E_INVALIDWORLDPATH		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0254)
#define VWOBJECT_E_CANTDESTROYAVATAR		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0255)
#define VWOBJECT_E_INVALIDDSURL			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0256)
#define VWOBJECT_E_NOTCLIENTMODULE		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0257)
#define VWOBJECT_E_NOTSERVERMODULE		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0258)
#define VWOBJECT_E_NOTSAFEFORSCRIPTING	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0259)
#define VWOBJECT_E_INVALIDPROFILEVERSION	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x025a)
#define VWOBJECT_E_INVALIDCLIENTVERSION	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x025b)
#define VWOBJECT_E_SCRIPTNOTFOUND		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x025c)
#define VWOBJECT_E_COMPONENTNOTFOUND		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x025d)
#define VWOBJECT_E_INVALIDONACCESSMETHOD	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x025e)
#define VWOBJECT_E_EXTERNALCALLONLY		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x025f)
#define VWOBJECT_E_EXCEEDEDOBJECTQUOTA	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0260)
#define VWOBJECT_E_CANTDESTROYEXEMPLAR	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0261)
#define VWOBJECT_E_INVALIDMETHODNAME		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0262)
#define VWOBJECT_E_INVALIDWORLDNAME		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0263)
#define VWOBJECT_E_INVALIDAVATARNAME		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0264)
#define VWOBJECT_E_INVALIDNAME			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0265)
#define VWOBJECT_E_NORECONNECTTOSAMEWORLD	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0266)
#define VWOBJECT_E_INVALIDPERMISSIONS	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0267)
#define VWOBJECT_E_INVALIDDIRECTORYSERVICEPAGE	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0268)
#define VWOBJECT_E_PARSESCRIPTERROR		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0269)
#define VWOBJECT_E_MODULESHUTDOWN		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x026A)
#define VWOBJECT_E_CANTCREATEAVATAR		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x026B)
#define VWOBJECT_E_RUNTIMESCRIPTERROR	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x026C)
#define VWOBJECT_E_UNAUTHENTICATEDCONNECTION	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x026D)
#define VWOBJECT_E_COPYINGNOTSUPPORTED	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x026E)


extern RPC_IF_HANDLE __MIDL_itf_vwobject_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vwobject_0000_v0_0_s_ifspec;

#ifndef __IWorld_INTERFACE_DEFINED__
#define __IWorld_INTERFACE_DEFINED__

/* interface IWorld */
/* [unique][helpstring][dual][uuid][object] */ 

#define	DISPID_WORLD_CONNECTINT	( 101 )

#define	DISPID_WORLD_CREATEAVATAR	( 102 )

#define	DISPID_WORLD_DISCONNECTINT	( 103 )

#define	DISPID_WORLD_SAVECHECKPOINT	( 104 )

#define	DISPID_WORLD_CREATEINSTANCEINT	( 105 )

#define	DISPID_WORLD_CREATEEXEMPLARINT	( 108 )

#define	DISPID_WORLD_DESTROYOBJECT	( 110 )

#define	DISPID_WORLD_CONVERTINSTANCETOEXEMPLAR	( 112 )

#define	DISPID_WORLD_CONVERTEXEMPLARTOINSTANCE	( 113 )

#define	DISPID_WORLD_CREATECOMMODULE	( 114 )

#define	DISPID_WORLD_CREATESCRIPTMODULE	( 116 )

#define	DISPID_WORLD_REMOVEMODULE	( 117 )

#define	DISPID_WORLD_SYNCHRONIZE	( 118 )

#define	DISPID_WORLD_ENABLELOGGING	( 119 )

#define	DISPID_WORLD_CREATEOBJECTINT	( 120 )

#define	DISPID_WORLD_DELETEOBJECTINT	( 121 )

#define	DISPID_WORLD_CREATECOMMODULEEX	( 122 )

#define	DISPID_WORLD_CREATESCRIPTMODULEEX	( 123 )

#define	DISPID_WORLD_CLONEINSTANCE	( 124 )

#define	DISPID_WORLD_REGISTERINTEREST	( 125 )

#define	DISPID_WORLD_UNREGISTERINTEREST	( 126 )

#define	DISPID_WORLD_SAVETOSCRIPT	( 128 )

#define	DISPID_WORLD_DISCONNECTAVATAR	( 129 )

#define	DISPID_WORLD_DISCONNECTCLIENT	( 130 )

#define	DISPID_WORLD_SAVECHECKPOINTAS	( 131 )

#define	DISPID_WORLD_UPDATEGLOBALPATHS	( 132 )

#define	CP_WIZARD	( 1 )

#define	CP_OWNER	( 2 )

#define	CP_WORLDOWNER	( 4 )

#define	CP_CALLER	( 8 )


EXTERN_C const IID IID_IWorld;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D9854-A417-11CF-9BF0-0080C7A56A8A")
    IWorld : public IObjectProperty
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Connect( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrPassword,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE CreateAvatar( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrPassword,
            /* [optional][in] */ VARIANT bstrGUID,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrNewName) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ConnectInt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrPassword,
            /* [in] */ VARIANT varProfile,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE DisconnectInt( void) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE DisconnectAvatar( 
            /* [in] */ IThing __RPC_FAR *pAvatar) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE DisconnectClient( 
            /* [in] */ IThing __RPC_FAR *pAvatar) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE Reconnect( 
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrPassword) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ReconnectExt( 
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrPassword) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ActivateRoom( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE DeactivateRoom( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Synchronize( void) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Database( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE OpenDatabase( 
            /* [in] */ BSTR bstr,
            /* [in] */ VARIANT_BOOL bCreate,
            /* [in] */ VARIANT_BOOL bOverwrite) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE SaveDatabase( void) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE SaveDatabaseAs( 
            BSTR bstrNewName) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE SaveToScript( 
            /* [in] */ BSTR bstrFilename) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ProcessLogRecord( 
            /* [in] */ IUnMarshallBuffer __RPC_FAR *pbuffer) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE EnableLogging( 
            /* [in] */ VARIANT_BOOL boolval) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_IsLoggingEnabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CreateObjectInt( 
            /* [in] */ long lval,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE DeleteObjectInt( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateInstance( 
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT varExemplar,
            /* [optional][in] */ VARIANT varContainer,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CreateInstanceExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT varExemplar,
            /* [in] */ IThing __RPC_FAR *pthingContainer,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CreateInstanceInt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IThing __RPC_FAR *pthingExemplar,
            /* [in] */ IThing __RPC_FAR *pthingContainer,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateExemplar( 
            /* [in] */ BSTR bstrType,
            /* [in] */ VARIANT varExemplar,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CreateExemplarEx( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrType,
            /* [in] */ VARIANT varExemplar,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CreateExemplarExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrType,
            /* [in] */ VARIANT varExemplar,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CreateExemplarInt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrType,
            /* [in] */ IThing __RPC_FAR *pthingExemplar,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CreateRootExemplar( 
            /* [in] */ BSTR bstrType,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE CloneInstance( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IThing __RPC_FAR *pthingSource,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE DestroyObject( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE DestroyObjectByID( 
            /* [in] */ long lval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ObjectByName( 
            /* [in] */ BSTR bstr,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_ObjectByID( 
            /* [in] */ long lval,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Exemplar( 
            /* [in] */ BSTR bstrType,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsValidExemplar( 
            /* [in] */ BSTR bstrType,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_Exemplars( 
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CreateObjectProperty( 
            /* [in] */ BSTR bstrProgID,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppproperty) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CreateObjectPropertyExt( 
            /* [in] */ REFCLSID clsid,
            /* [in] */ IUnknown __RPC_FAR *punk,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppproperty) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CreateMethodByDispID( 
            /* [in] */ long lFlags,
            /* [in] */ VARIANT varModule,
            /* [in] */ DISPID dispid,
            /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateMethod( 
            /* [in] */ long lFlags,
            /* [in] */ VARIANT varModule,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateInlineMethod( 
            /* [in] */ long lFlags,
            /* [in] */ BSTR bstrLanguage,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrCode,
            /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CreateCOMModule( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrProgID,
            /* [in] */ long lFlags,
            /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE CreateScriptModule( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrScriptPath,
            /* [in] */ long lFlags,
            /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CreateCOMModuleEx( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrProgID,
            /* [in] */ long lFlags,
            /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CreateScriptModuleEx( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrScriptPath,
            /* [in] */ long lFlags,
            /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE RemoveModule( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Module( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsValidModule( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_Modules( 
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropmap) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CreateTool( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrProgID,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CreateToolFromInterface( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IUnknown __RPC_FAR *punk) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CreateToolExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ REFCLSID clsid,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE RemoveTool( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IsValidTool( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_Tool( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ToolExt( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Tools( 
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropmap) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE InitializeTools( 
            /* [in] */ IPropertyMap __RPC_FAR *ppropmap) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE InitializeConnection( 
            /* [in] */ IVWCommConnection __RPC_FAR *pvwcommconnection) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE DisconnectConnection( 
            /* [in] */ IVWCommConnection __RPC_FAR *pvwcommconnection) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE OnOpenWorld( 
            /* [in] */ VARIANT_BOOL bPersist) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE OnCreateWorld( void) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ProcessMessage( 
            /* [in] */ IVWCommMessage __RPC_FAR *pmessage) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ProcessMessageExt( 
            /* [in] */ unsigned char ch,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *pUBuffer,
            /* [out] */ HRESULT __RPC_FAR *recvhr,
            /* [out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Connection( 
            /* [retval][out] */ IVWCommConnection __RPC_FAR *__RPC_FAR *ppvwcommconnection) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Connection( 
            /* [in] */ IVWCommConnection __RPC_FAR *pvwcommconnection) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ShouldExecute( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ShouldExecuteSync( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ShouldRemote( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ShouldRemoteSync( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ShouldNotify( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_LocalMode( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_LocalMode( 
            /* [in] */ VARIANT_BOOL boolval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_FreezeEvents( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_FreezeEvents( 
            /* [in] */ VARIANT_BOOL boolval) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_EnableSecurity( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE EnableTrace( 
            /* [in] */ VARIANT_BOOL boolval) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_IsTraceEnabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_TraceLevel( 
            /* [in] */ BSTR bstr,
            /* [retval][out] */ int __RPC_FAR *ival) = 0;
        
        virtual /* [helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_TraceLevel( 
            /* [in] */ BSTR bstr,
            /* [in] */ int ival) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE RemoveTraceLevel( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Trace( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE Report( 
            /* [in] */ BSTR bstr,
            /* [in] */ long lType) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Print( 
            /* [in] */ VARIANT varAnything) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Format( 
            /* [in] */ BSTR bstrIn,
            /* [in] */ IPropertyList __RPC_FAR *pObjects,
            /* [in] */ IThing __RPC_FAR *pPOV,
            /* [retval][out] */ BSTR __RPC_FAR *bstrOut) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FormatSentences( 
            /* [in] */ BSTR bstrIn,
            /* [in] */ IPropertyList __RPC_FAR *pObjects,
            /* [in] */ IThing __RPC_FAR *pPOV,
            /* [retval][out] */ BSTR __RPC_FAR *bstrOut) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_IsConnected( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ServerSide( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_ServerSide( 
            /* [in] */ VARIANT_BOOL boolval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Avatar( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_AvatarByGUID( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Global( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Owner( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_User( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_User( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_This( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_This( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Caller( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Caller( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Super( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Super( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_SecurityContext( 
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_SecurityContext( 
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_IsInPropChange( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_IsInPropChange( 
            /* [in] */ VARIANT_BOOL boolval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AbortEvent( void) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_IsEventAborted( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_IsEventAborted( 
            /* [in] */ VARIANT_BOOL boolval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsExemplar( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsAvatar( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsRoom( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE LockWorld( void) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE UnlockWorld( void) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE NotifyUserConnect( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE NotifyUserDisconnect( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FireUIEvent( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrEventName,
            /* [optional][in] */ VARIANT varArg) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ShowHTML( 
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrHtml,
            /* [defaultvalue][optional][in] */ VARIANT cx,
            /* [defaultvalue][optional][in] */ VARIANT cy) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ShowHTMLEx( 
            /* [in] */ BSTR bstrWinName,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrHtml,
            /* [in] */ VARIANT dispArg,
            /* [defaultvalue][optional][in] */ VARIANT x,
            /* [defaultvalue][optional][in] */ VARIANT y,
            /* [defaultvalue][optional][in] */ VARIANT cx,
            /* [defaultvalue][optional][in] */ VARIANT cy) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ShowHTMLExt( 
            /* [in] */ BSTR bstrWinName,
            /* [in] */ BSTR bstrHtml,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ VARIANT_BOOL bSafeMode,
            /* [in] */ IDispatch __RPC_FAR *pdispArg,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ int cx,
            /* [in] */ int cy) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ShowURL( 
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrUrl,
            /* [defaultvalue][optional][in] */ VARIANT cx,
            /* [defaultvalue][optional][in] */ VARIANT cy) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ShowURLEx( 
            /* [in] */ BSTR bstrWinName,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrUrl,
            /* [in] */ VARIANT dispArg,
            /* [defaultvalue][optional][in] */ VARIANT x,
            /* [defaultvalue][optional][in] */ VARIANT y,
            /* [defaultvalue][optional][in] */ VARIANT cx,
            /* [defaultvalue][optional][in] */ VARIANT cy) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ShowURLExt( 
            /* [in] */ BSTR bstrWinName,
            /* [in] */ VARIANT_BOOL bPopup,
            /* [in] */ BSTR bstrUrl,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ VARIANT_BOOL bSafeMode,
            /* [in] */ IDispatch __RPC_FAR *pdispArg,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ int cx,
            /* [in] */ int cy) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE NotifyInvokeMethod( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstr,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE NotifyPropertyChanged( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstr,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE NotifyMethodChanged( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstr,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE NotifyModuleChanged( 
            /* [in] */ BSTR bstr,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendContainer( 
            /* [in] */ long lval,
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendContents( 
            /* [in] */ long lval,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ VARIANT_BOOL bForceOpen) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendWorld( 
            /* [in] */ long lval) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendExemplar( 
            /* [in] */ long lval,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendThingByID( 
            /* [in] */ long lval,
            /* [in] */ long lID) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendThing( 
            /* [in] */ long lval,
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendModule( 
            /* [in] */ long lval,
            /* [in] */ IModule __RPC_FAR *pmodule,
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendWorldCommand( 
            /* [in] */ VARIANT_BOOL bReply,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [out] */ HRESULT __RPC_FAR *recvhr,
            /* [out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendThingCommand( 
            /* [in] */ long lFlags,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendObjectPropertyCommand( 
            /* [in] */ long lFlags,
            /* [in] */ IObjectProperty __RPC_FAR *pprop,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendWorldCommandExt( 
            /* [in] */ VARIANT_BOOL bReply,
            /* [in] */ VARIANT_BOOL bSync,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [out] */ HRESULT __RPC_FAR *recvhr,
            /* [out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendThingCommandExt( 
            /* [in] */ VARIANT_BOOL bReply,
            /* [in] */ VARIANT_BOOL bSync,
            /* [in] */ long lFlags,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [out] */ HRESULT __RPC_FAR *recvhr,
            /* [out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendObjectPropertyCommandExt( 
            /* [in] */ VARIANT_BOOL bReply,
            /* [in] */ VARIANT_BOOL bSync,
            /* [in] */ long lFlags,
            /* [in] */ IObjectProperty __RPC_FAR *pprop,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [out] */ HRESULT __RPC_FAR *recvhr,
            /* [out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendModuleCommand( 
            /* [in] */ IModule __RPC_FAR *pmodule,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendMethodCommand( 
            /* [in] */ IMethod __RPC_FAR *pmethod,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE LogThingCommand( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE LogWorldCommand( 
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE WriteCommandHeader( 
            /* [in] */ IMarshallBuffer __RPC_FAR *pBuffer,
            /* [in] */ unsigned char ucReply,
            /* [in] */ unsigned char ucCommand) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE WriteCommandCallstack( 
            /* [in] */ IMarshallBuffer __RPC_FAR *pBuffer) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE RegisterInterest( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE UnregisterInterest( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE MarshallThing( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE UnMarshallThing( 
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
            /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE MarshallProperty( 
            /* [in] */ VARIANT var,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE UnMarshallProperty( 
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
            /* [out] */ VARIANT __RPC_FAR *pvar) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE MarshallObjectProperty( 
            /* [in] */ IObjectProperty __RPC_FAR *pprop,
            /* [in] */ long lHint,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE UnMarshallObjectProperty( 
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
            /* [out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppprop) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE MarshallModule( 
            /* [in] */ IModule __RPC_FAR *pmodule,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE UnMarshallModule( 
            /* [in] */ REFCLSID clsid,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
            /* [out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE MarshallDISPPARAMS( 
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer,
            /* [in] */ DWORD dwOptions) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE UnMarshallDISPPARAMS( 
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pdispparams) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE MarshallPropertyToScript( 
            /* [in] */ VARIANT var,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CheckPolicy( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ unsigned long lFlags,
            /* [optional][in] */ VARIANT var,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAccessOk) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CheckPolicyExt( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ unsigned long lFlags,
            /* [optional][in] */ VARIANT var) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ValidateProperty( 
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ValidateDISPPARAMS( 
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ValidateObjectSafety( 
            /* [in] */ REFCLSID clsid,
            /* [in] */ IUnknown __RPC_FAR *punk,
            /* [in] */ DWORD dwOptions) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ValidatePropertyName( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ValidateMethodName( 
            /* [in] */ BSTR bstrName,
            /* [in] */ long psbits,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ValidateInstanceName( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ValidateExemplarType( 
            /* [in] */ BSTR bstrType,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ValidateWorldName( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ValidateAvatarName( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ValidatePropertyNameExt( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ValidateMethodNameExt( 
            /* [in] */ BSTR bstrName,
            /* [in] */ long psbits) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ValidateInstanceNameExt( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ValidateExemplarTypeExt( 
            /* [in] */ BSTR bstrType) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ValidateWorldNameExt( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ValidateAvatarNameExt( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE CloneProperty( 
            /* [in] */ VARIANT source,
            /* [retval][out] */ VARIANT __RPC_FAR *pDest) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE DumpProperty( 
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddCLSIDToTable( 
            /* [in] */ REFCLSID clsid) = 0;
        
        virtual /* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE UpdateGlobalPaths( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWorldVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IWorld __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IWorld __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IWorld __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IWorld __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IWorld __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IWorld __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IWorld __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IWorld __RPC_FAR * This);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RefCount )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CLSID )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IID )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parent )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyName )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyName )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Security )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Security )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsOfType )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToAscii )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            IWorld __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            IWorld __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallToScript )( 
            IWorld __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Connect )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrPassword,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disconnect )( 
            IWorld __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateAvatar )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrPassword,
            /* [optional][in] */ VARIANT bstrGUID,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrNewName);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConnectInt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrPassword,
            /* [in] */ VARIANT varProfile,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DisconnectInt )( 
            IWorld __RPC_FAR * This);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DisconnectAvatar )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pAvatar);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DisconnectClient )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pAvatar);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reconnect )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrPassword);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReconnectExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrPassword);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ActivateRoom )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeactivateRoom )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Synchronize )( 
            IWorld __RPC_FAR * This);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Database )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OpenDatabase )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ VARIANT_BOOL bCreate,
            /* [in] */ VARIANT_BOOL bOverwrite);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveDatabase )( 
            IWorld __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveDatabaseAs )( 
            IWorld __RPC_FAR * This,
            BSTR bstrNewName);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveToScript )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrFilename);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessLogRecord )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *pbuffer);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnableLogging )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL boolval);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsLoggingEnabled )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateObjectInt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lval,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteObjectInt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateInstance )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT varExemplar,
            /* [optional][in] */ VARIANT varContainer,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateInstanceExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT varExemplar,
            /* [in] */ IThing __RPC_FAR *pthingContainer,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateInstanceInt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IThing __RPC_FAR *pthingExemplar,
            /* [in] */ IThing __RPC_FAR *pthingContainer,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateExemplar )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrType,
            /* [in] */ VARIANT varExemplar,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateExemplarEx )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrType,
            /* [in] */ VARIANT varExemplar,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateExemplarExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrType,
            /* [in] */ VARIANT varExemplar,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateExemplarInt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrType,
            /* [in] */ IThing __RPC_FAR *pthingExemplar,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateRootExemplar )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrType,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloneInstance )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IThing __RPC_FAR *pthingSource,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DestroyObject )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DestroyObjectByID )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ObjectByName )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ObjectByID )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lval,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Exemplar )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrType,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValidExemplar )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrType,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Exemplars )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateObjectProperty )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrProgID,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateObjectPropertyExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ REFCLSID clsid,
            /* [in] */ IUnknown __RPC_FAR *punk,
            /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppproperty);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateMethodByDispID )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lFlags,
            /* [in] */ VARIANT varModule,
            /* [in] */ DISPID dispid,
            /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateMethod )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lFlags,
            /* [in] */ VARIANT varModule,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateInlineMethod )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lFlags,
            /* [in] */ BSTR bstrLanguage,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrCode,
            /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateCOMModule )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrProgID,
            /* [in] */ long lFlags,
            /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateScriptModule )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrScriptPath,
            /* [in] */ long lFlags,
            /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateCOMModuleEx )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrProgID,
            /* [in] */ long lFlags,
            /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateScriptModuleEx )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrScriptPath,
            /* [in] */ long lFlags,
            /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveModule )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Module )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValidModule )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Modules )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropmap);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateTool )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrProgID,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateToolFromInterface )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IUnknown __RPC_FAR *punk);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateToolExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ REFCLSID clsid,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveTool )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValidTool )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Tool )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ToolExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Tools )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropmap);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitializeTools )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IPropertyMap __RPC_FAR *ppropmap);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitializeConnection )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IVWCommConnection __RPC_FAR *pvwcommconnection);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DisconnectConnection )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IVWCommConnection __RPC_FAR *pvwcommconnection);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnOpenWorld )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bPersist);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnCreateWorld )( 
            IWorld __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessMessage )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IVWCommMessage __RPC_FAR *pmessage);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessMessageExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ unsigned char ch,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *pUBuffer,
            /* [out] */ HRESULT __RPC_FAR *recvhr,
            /* [out] */ VARIANT __RPC_FAR *pvar);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Connection )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IVWCommConnection __RPC_FAR *__RPC_FAR *ppvwcommconnection);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Connection )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IVWCommConnection __RPC_FAR *pvwcommconnection);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ShouldExecute )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ShouldExecuteSync )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ShouldRemote )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ShouldRemoteSync )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ShouldNotify )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LocalMode )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LocalMode )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL boolval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FreezeEvents )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FreezeEvents )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL boolval);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_EnableSecurity )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnableTrace )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL boolval);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsTraceEnabled )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TraceLevel )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ int __RPC_FAR *ival);
        
        /* [helpstring][propput][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TraceLevel )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ int ival);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveTraceLevel )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Trace )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Report )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ long lType);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Print )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT varAnything);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Format )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrIn,
            /* [in] */ IPropertyList __RPC_FAR *pObjects,
            /* [in] */ IThing __RPC_FAR *pPOV,
            /* [retval][out] */ BSTR __RPC_FAR *bstrOut);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FormatSentences )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrIn,
            /* [in] */ IPropertyList __RPC_FAR *pObjects,
            /* [in] */ IThing __RPC_FAR *pPOV,
            /* [retval][out] */ BSTR __RPC_FAR *bstrOut);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsConnected )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ServerSide )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ServerSide )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL boolval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Avatar )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AvatarByGUID )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Global )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Owner )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_User )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_User )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_This )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_This )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Caller )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Caller )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Super )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Super )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SecurityContext )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SecurityContext )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsInPropChange )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_IsInPropChange )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL boolval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AbortEvent )( 
            IWorld __RPC_FAR * This);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEventAborted )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_IsEventAborted )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL boolval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsExemplar )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsAvatar )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsRoom )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LockWorld )( 
            IWorld __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnlockWorld )( 
            IWorld __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NotifyUserConnect )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NotifyUserDisconnect )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FireUIEvent )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrEventName,
            /* [optional][in] */ VARIANT varArg);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowHTML )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrHtml,
            /* [defaultvalue][optional][in] */ VARIANT cx,
            /* [defaultvalue][optional][in] */ VARIANT cy);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowHTMLEx )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrWinName,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrHtml,
            /* [in] */ VARIANT dispArg,
            /* [defaultvalue][optional][in] */ VARIANT x,
            /* [defaultvalue][optional][in] */ VARIANT y,
            /* [defaultvalue][optional][in] */ VARIANT cx,
            /* [defaultvalue][optional][in] */ VARIANT cy);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowHTMLExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrWinName,
            /* [in] */ BSTR bstrHtml,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ VARIANT_BOOL bSafeMode,
            /* [in] */ IDispatch __RPC_FAR *pdispArg,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ int cx,
            /* [in] */ int cy);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowURL )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrUrl,
            /* [defaultvalue][optional][in] */ VARIANT cx,
            /* [defaultvalue][optional][in] */ VARIANT cy);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowURLEx )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrWinName,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrUrl,
            /* [in] */ VARIANT dispArg,
            /* [defaultvalue][optional][in] */ VARIANT x,
            /* [defaultvalue][optional][in] */ VARIANT y,
            /* [defaultvalue][optional][in] */ VARIANT cx,
            /* [defaultvalue][optional][in] */ VARIANT cy);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowURLExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrWinName,
            /* [in] */ VARIANT_BOOL bPopup,
            /* [in] */ BSTR bstrUrl,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ VARIANT_BOOL bSafeMode,
            /* [in] */ IDispatch __RPC_FAR *pdispArg,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ int cx,
            /* [in] */ int cy);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NotifyInvokeMethod )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstr,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NotifyPropertyChanged )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstr,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NotifyMethodChanged )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstr,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NotifyModuleChanged )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendContainer )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lval,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendContents )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lval,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ VARIANT_BOOL bForceOpen);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendWorld )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lval);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendExemplar )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lval,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendThingByID )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lval,
            /* [in] */ long lID);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendThing )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lval,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendModule )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lval,
            /* [in] */ IModule __RPC_FAR *pmodule,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendWorldCommand )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bReply,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [out] */ HRESULT __RPC_FAR *recvhr,
            /* [out] */ VARIANT __RPC_FAR *pvar);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendThingCommand )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lFlags,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendObjectPropertyCommand )( 
            IWorld __RPC_FAR * This,
            /* [in] */ long lFlags,
            /* [in] */ IObjectProperty __RPC_FAR *pprop,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendWorldCommandExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bReply,
            /* [in] */ VARIANT_BOOL bSync,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [out] */ HRESULT __RPC_FAR *recvhr,
            /* [out] */ VARIANT __RPC_FAR *pvar);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendThingCommandExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bReply,
            /* [in] */ VARIANT_BOOL bSync,
            /* [in] */ long lFlags,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [out] */ HRESULT __RPC_FAR *recvhr,
            /* [out] */ VARIANT __RPC_FAR *pvar);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendObjectPropertyCommandExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bReply,
            /* [in] */ VARIANT_BOOL bSync,
            /* [in] */ long lFlags,
            /* [in] */ IObjectProperty __RPC_FAR *pprop,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [out] */ HRESULT __RPC_FAR *recvhr,
            /* [out] */ VARIANT __RPC_FAR *pvar);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendModuleCommand )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IModule __RPC_FAR *pmodule,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendMethodCommand )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IMethod __RPC_FAR *pmethod,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LogThingCommand )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LogWorldCommand )( 
            IWorld __RPC_FAR * This,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteCommandHeader )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IMarshallBuffer __RPC_FAR *pBuffer,
            /* [in] */ unsigned char ucReply,
            /* [in] */ unsigned char ucCommand);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteCommandCallstack )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IMarshallBuffer __RPC_FAR *pBuffer);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterInterest )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnregisterInterest )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallThing )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshallThing )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
            /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallProperty )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT var,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshallProperty )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
            /* [out] */ VARIANT __RPC_FAR *pvar);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallObjectProperty )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pprop,
            /* [in] */ long lHint,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshallObjectProperty )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
            /* [out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppprop);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallModule )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IModule __RPC_FAR *pmodule,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshallModule )( 
            IWorld __RPC_FAR * This,
            /* [in] */ REFCLSID clsid,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
            /* [out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallDISPPARAMS )( 
            IWorld __RPC_FAR * This,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer,
            /* [in] */ DWORD dwOptions);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshallDISPPARAMS )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pdispparams);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallPropertyToScript )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT var,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckPolicy )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ unsigned long lFlags,
            /* [optional][in] */ VARIANT var,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAccessOk);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckPolicyExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ unsigned long lFlags,
            /* [optional][in] */ VARIANT var);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateProperty )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT var);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateDISPPARAMS )( 
            IWorld __RPC_FAR * This,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateObjectSafety )( 
            IWorld __RPC_FAR * This,
            /* [in] */ REFCLSID clsid,
            /* [in] */ IUnknown __RPC_FAR *punk,
            /* [in] */ DWORD dwOptions);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidatePropertyName )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateMethodName )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ long psbits,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateInstanceName )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateExemplarType )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrType,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateWorldName )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateAvatarName )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidatePropertyNameExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateMethodNameExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ long psbits);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateInstanceNameExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateExemplarTypeExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrType);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateWorldNameExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateAvatarNameExt )( 
            IWorld __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloneProperty )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT source,
            /* [retval][out] */ VARIANT __RPC_FAR *pDest);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DumpProperty )( 
            IWorld __RPC_FAR * This,
            /* [in] */ VARIANT var);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Version )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStatus )( 
            IWorld __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddCLSIDToTable )( 
            IWorld __RPC_FAR * This,
            /* [in] */ REFCLSID clsid);
        
        /* [id][helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateGlobalPaths )( 
            IWorld __RPC_FAR * This);
        
        END_INTERFACE
    } IWorldVtbl;

    interface IWorld
    {
        CONST_VTBL struct IWorldVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWorld_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWorld_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWorld_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWorld_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWorld_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWorld_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWorld_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWorld_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IWorld_get_RefCount(This,pdwRef)	\
    (This)->lpVtbl -> get_RefCount(This,pdwRef)

#define IWorld_get_CLSID(This,pclsid)	\
    (This)->lpVtbl -> get_CLSID(This,pclsid)

#define IWorld_get_IID(This,piid)	\
    (This)->lpVtbl -> get_IID(This,piid)

#define IWorld_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IWorld_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define IWorld_get_Parent(This,ppthing)	\
    (This)->lpVtbl -> get_Parent(This,ppthing)

#define IWorld_put_Parent(This,pthing)	\
    (This)->lpVtbl -> put_Parent(This,pthing)

#define IWorld_get_PropertyName(This,pbstr)	\
    (This)->lpVtbl -> get_PropertyName(This,pbstr)

#define IWorld_put_PropertyName(This,bstr)	\
    (This)->lpVtbl -> put_PropertyName(This,bstr)

#define IWorld_get_Type(This,pbstr)	\
    (This)->lpVtbl -> get_Type(This,pbstr)

#define IWorld_put_Type(This,bstr)	\
    (This)->lpVtbl -> put_Type(This,bstr)

#define IWorld_get_Security(This,ppSecurity)	\
    (This)->lpVtbl -> get_Security(This,ppSecurity)

#define IWorld_put_Security(This,pSecurity)	\
    (This)->lpVtbl -> put_Security(This,pSecurity)

#define IWorld_IsOfType(This,bstr,pbool)	\
    (This)->lpVtbl -> IsOfType(This,bstr,pbool)

#define IWorld_ToAscii(This,pbstr)	\
    (This)->lpVtbl -> ToAscii(This,pbstr)

#define IWorld_Dump(This)	\
    (This)->lpVtbl -> Dump(This)

#define IWorld_Clone(This,pobjectproperty)	\
    (This)->lpVtbl -> Clone(This,pobjectproperty)

#define IWorld_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define IWorld_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define IWorld_MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)


#define IWorld_Connect(This,bstrName,bstrPassword,ppthing)	\
    (This)->lpVtbl -> Connect(This,bstrName,bstrPassword,ppthing)

#define IWorld_Disconnect(This)	\
    (This)->lpVtbl -> Disconnect(This)

#define IWorld_CreateAvatar(This,bstrName,bstrPassword,bstrGUID,pbstrNewName)	\
    (This)->lpVtbl -> CreateAvatar(This,bstrName,bstrPassword,bstrGUID,pbstrNewName)

#define IWorld_ConnectInt(This,bstrName,bstrPassword,varProfile,ppthing)	\
    (This)->lpVtbl -> ConnectInt(This,bstrName,bstrPassword,varProfile,ppthing)

#define IWorld_DisconnectInt(This)	\
    (This)->lpVtbl -> DisconnectInt(This)

#define IWorld_DisconnectAvatar(This,pAvatar)	\
    (This)->lpVtbl -> DisconnectAvatar(This,pAvatar)

#define IWorld_DisconnectClient(This,pAvatar)	\
    (This)->lpVtbl -> DisconnectClient(This,pAvatar)

#define IWorld_Reconnect(This,bstrURL,bstrName,bstrPassword)	\
    (This)->lpVtbl -> Reconnect(This,bstrURL,bstrName,bstrPassword)

#define IWorld_ReconnectExt(This,bstrURL,bstrName,bstrPassword)	\
    (This)->lpVtbl -> ReconnectExt(This,bstrURL,bstrName,bstrPassword)

#define IWorld_ActivateRoom(This,pthing)	\
    (This)->lpVtbl -> ActivateRoom(This,pthing)

#define IWorld_DeactivateRoom(This,pthing)	\
    (This)->lpVtbl -> DeactivateRoom(This,pthing)

#define IWorld_Synchronize(This)	\
    (This)->lpVtbl -> Synchronize(This)

#define IWorld_get_Database(This,pbstr)	\
    (This)->lpVtbl -> get_Database(This,pbstr)

#define IWorld_OpenDatabase(This,bstr,bCreate,bOverwrite)	\
    (This)->lpVtbl -> OpenDatabase(This,bstr,bCreate,bOverwrite)

#define IWorld_SaveDatabase(This)	\
    (This)->lpVtbl -> SaveDatabase(This)

#define IWorld_SaveDatabaseAs(This,bstrNewName)	\
    (This)->lpVtbl -> SaveDatabaseAs(This,bstrNewName)

#define IWorld_SaveToScript(This,bstrFilename)	\
    (This)->lpVtbl -> SaveToScript(This,bstrFilename)

#define IWorld_ProcessLogRecord(This,pbuffer)	\
    (This)->lpVtbl -> ProcessLogRecord(This,pbuffer)

#define IWorld_EnableLogging(This,boolval)	\
    (This)->lpVtbl -> EnableLogging(This,boolval)

#define IWorld_get_IsLoggingEnabled(This,pbool)	\
    (This)->lpVtbl -> get_IsLoggingEnabled(This,pbool)

#define IWorld_CreateObjectInt(This,lval,ppthing)	\
    (This)->lpVtbl -> CreateObjectInt(This,lval,ppthing)

#define IWorld_DeleteObjectInt(This,pthing)	\
    (This)->lpVtbl -> DeleteObjectInt(This,pthing)

#define IWorld_CreateInstance(This,bstrName,varExemplar,varContainer,ppthing)	\
    (This)->lpVtbl -> CreateInstance(This,bstrName,varExemplar,varContainer,ppthing)

#define IWorld_CreateInstanceExt(This,bstrName,varExemplar,pthingContainer,ppthing)	\
    (This)->lpVtbl -> CreateInstanceExt(This,bstrName,varExemplar,pthingContainer,ppthing)

#define IWorld_CreateInstanceInt(This,bstrName,pthingExemplar,pthingContainer,ppthing)	\
    (This)->lpVtbl -> CreateInstanceInt(This,bstrName,pthingExemplar,pthingContainer,ppthing)

#define IWorld_CreateExemplar(This,bstrType,varExemplar,ppthing)	\
    (This)->lpVtbl -> CreateExemplar(This,bstrType,varExemplar,ppthing)

#define IWorld_CreateExemplarEx(This,bstrName,bstrType,varExemplar,ppthing)	\
    (This)->lpVtbl -> CreateExemplarEx(This,bstrName,bstrType,varExemplar,ppthing)

#define IWorld_CreateExemplarExt(This,bstrName,bstrType,varExemplar,ppthing)	\
    (This)->lpVtbl -> CreateExemplarExt(This,bstrName,bstrType,varExemplar,ppthing)

#define IWorld_CreateExemplarInt(This,bstrName,bstrType,pthingExemplar,ppthing)	\
    (This)->lpVtbl -> CreateExemplarInt(This,bstrName,bstrType,pthingExemplar,ppthing)

#define IWorld_CreateRootExemplar(This,bstrType,ppthing)	\
    (This)->lpVtbl -> CreateRootExemplar(This,bstrType,ppthing)

#define IWorld_CloneInstance(This,bstrName,pthingSource,ppthing)	\
    (This)->lpVtbl -> CloneInstance(This,bstrName,pthingSource,ppthing)

#define IWorld_DestroyObject(This,pthing)	\
    (This)->lpVtbl -> DestroyObject(This,pthing)

#define IWorld_DestroyObjectByID(This,lval)	\
    (This)->lpVtbl -> DestroyObjectByID(This,lval)

#define IWorld_get_ObjectByName(This,bstr,ppthing)	\
    (This)->lpVtbl -> get_ObjectByName(This,bstr,ppthing)

#define IWorld_get_ObjectByID(This,lval,ppthing)	\
    (This)->lpVtbl -> get_ObjectByID(This,lval,ppthing)

#define IWorld_get_Exemplar(This,bstrType,ppthing)	\
    (This)->lpVtbl -> get_Exemplar(This,bstrType,ppthing)

#define IWorld_IsValidExemplar(This,bstrType,pbool)	\
    (This)->lpVtbl -> IsValidExemplar(This,bstrType,pbool)

#define IWorld_get_Exemplars(This,pppropertymap)	\
    (This)->lpVtbl -> get_Exemplars(This,pppropertymap)

#define IWorld_CreateObjectProperty(This,bstrProgID,ppproperty)	\
    (This)->lpVtbl -> CreateObjectProperty(This,bstrProgID,ppproperty)

#define IWorld_CreateObjectPropertyExt(This,clsid,punk,ppproperty)	\
    (This)->lpVtbl -> CreateObjectPropertyExt(This,clsid,punk,ppproperty)

#define IWorld_CreateMethodByDispID(This,lFlags,varModule,dispid,ppmethod)	\
    (This)->lpVtbl -> CreateMethodByDispID(This,lFlags,varModule,dispid,ppmethod)

#define IWorld_CreateMethod(This,lFlags,varModule,bstrName,ppmethod)	\
    (This)->lpVtbl -> CreateMethod(This,lFlags,varModule,bstrName,ppmethod)

#define IWorld_CreateInlineMethod(This,lFlags,bstrLanguage,bstrName,bstrCode,ppmethod)	\
    (This)->lpVtbl -> CreateInlineMethod(This,lFlags,bstrLanguage,bstrName,bstrCode,ppmethod)

#define IWorld_CreateCOMModule(This,bstrName,bstrProgID,lFlags,ppmodule)	\
    (This)->lpVtbl -> CreateCOMModule(This,bstrName,bstrProgID,lFlags,ppmodule)

#define IWorld_CreateScriptModule(This,bstrName,bstrScriptPath,lFlags,ppmodule)	\
    (This)->lpVtbl -> CreateScriptModule(This,bstrName,bstrScriptPath,lFlags,ppmodule)

#define IWorld_CreateCOMModuleEx(This,bstrName,bstrProgID,lFlags,ppmodule)	\
    (This)->lpVtbl -> CreateCOMModuleEx(This,bstrName,bstrProgID,lFlags,ppmodule)

#define IWorld_CreateScriptModuleEx(This,bstrName,bstrScriptPath,lFlags,ppmodule)	\
    (This)->lpVtbl -> CreateScriptModuleEx(This,bstrName,bstrScriptPath,lFlags,ppmodule)

#define IWorld_RemoveModule(This,bstrName)	\
    (This)->lpVtbl -> RemoveModule(This,bstrName)

#define IWorld_get_Module(This,bstrName,ppmodule)	\
    (This)->lpVtbl -> get_Module(This,bstrName,ppmodule)

#define IWorld_IsValidModule(This,bstrName,pbool)	\
    (This)->lpVtbl -> IsValidModule(This,bstrName,pbool)

#define IWorld_get_Modules(This,pppropmap)	\
    (This)->lpVtbl -> get_Modules(This,pppropmap)

#define IWorld_CreateTool(This,bstrName,bstrProgID,ppunk)	\
    (This)->lpVtbl -> CreateTool(This,bstrName,bstrProgID,ppunk)

#define IWorld_CreateToolFromInterface(This,bstrName,punk)	\
    (This)->lpVtbl -> CreateToolFromInterface(This,bstrName,punk)

#define IWorld_CreateToolExt(This,bstrName,clsid,ppunk)	\
    (This)->lpVtbl -> CreateToolExt(This,bstrName,clsid,ppunk)

#define IWorld_RemoveTool(This,bstrName)	\
    (This)->lpVtbl -> RemoveTool(This,bstrName)

#define IWorld_IsValidTool(This,bstrName,pbool)	\
    (This)->lpVtbl -> IsValidTool(This,bstrName,pbool)

#define IWorld_get_Tool(This,bstrName,ppunk)	\
    (This)->lpVtbl -> get_Tool(This,bstrName,ppunk)

#define IWorld_get_ToolExt(This,bstrName,ppunk)	\
    (This)->lpVtbl -> get_ToolExt(This,bstrName,ppunk)

#define IWorld_get_Tools(This,pppropmap)	\
    (This)->lpVtbl -> get_Tools(This,pppropmap)

#define IWorld_InitializeTools(This,ppropmap)	\
    (This)->lpVtbl -> InitializeTools(This,ppropmap)

#define IWorld_InitializeConnection(This,pvwcommconnection)	\
    (This)->lpVtbl -> InitializeConnection(This,pvwcommconnection)

#define IWorld_DisconnectConnection(This,pvwcommconnection)	\
    (This)->lpVtbl -> DisconnectConnection(This,pvwcommconnection)

#define IWorld_OnOpenWorld(This,bPersist)	\
    (This)->lpVtbl -> OnOpenWorld(This,bPersist)

#define IWorld_OnCreateWorld(This)	\
    (This)->lpVtbl -> OnCreateWorld(This)

#define IWorld_ProcessMessage(This,pmessage)	\
    (This)->lpVtbl -> ProcessMessage(This,pmessage)

#define IWorld_ProcessMessageExt(This,ch,pUBuffer,recvhr,pvar)	\
    (This)->lpVtbl -> ProcessMessageExt(This,ch,pUBuffer,recvhr,pvar)

#define IWorld_get_Connection(This,ppvwcommconnection)	\
    (This)->lpVtbl -> get_Connection(This,ppvwcommconnection)

#define IWorld_put_Connection(This,pvwcommconnection)	\
    (This)->lpVtbl -> put_Connection(This,pvwcommconnection)

#define IWorld_get_ShouldExecute(This,pbool)	\
    (This)->lpVtbl -> get_ShouldExecute(This,pbool)

#define IWorld_get_ShouldExecuteSync(This,pbool)	\
    (This)->lpVtbl -> get_ShouldExecuteSync(This,pbool)

#define IWorld_get_ShouldRemote(This,pbool)	\
    (This)->lpVtbl -> get_ShouldRemote(This,pbool)

#define IWorld_get_ShouldRemoteSync(This,pbool)	\
    (This)->lpVtbl -> get_ShouldRemoteSync(This,pbool)

#define IWorld_get_ShouldNotify(This,pbool)	\
    (This)->lpVtbl -> get_ShouldNotify(This,pbool)

#define IWorld_get_LocalMode(This,pbool)	\
    (This)->lpVtbl -> get_LocalMode(This,pbool)

#define IWorld_put_LocalMode(This,boolval)	\
    (This)->lpVtbl -> put_LocalMode(This,boolval)

#define IWorld_get_FreezeEvents(This,pbool)	\
    (This)->lpVtbl -> get_FreezeEvents(This,pbool)

#define IWorld_put_FreezeEvents(This,boolval)	\
    (This)->lpVtbl -> put_FreezeEvents(This,boolval)

#define IWorld_get_EnableSecurity(This,pbool)	\
    (This)->lpVtbl -> get_EnableSecurity(This,pbool)

#define IWorld_EnableTrace(This,boolval)	\
    (This)->lpVtbl -> EnableTrace(This,boolval)

#define IWorld_get_IsTraceEnabled(This,pbool)	\
    (This)->lpVtbl -> get_IsTraceEnabled(This,pbool)

#define IWorld_get_TraceLevel(This,bstr,ival)	\
    (This)->lpVtbl -> get_TraceLevel(This,bstr,ival)

#define IWorld_put_TraceLevel(This,bstr,ival)	\
    (This)->lpVtbl -> put_TraceLevel(This,bstr,ival)

#define IWorld_RemoveTraceLevel(This,bstr)	\
    (This)->lpVtbl -> RemoveTraceLevel(This,bstr)

#define IWorld_Trace(This,bstr)	\
    (This)->lpVtbl -> Trace(This,bstr)

#define IWorld_Report(This,bstr,lType)	\
    (This)->lpVtbl -> Report(This,bstr,lType)

#define IWorld_Print(This,varAnything)	\
    (This)->lpVtbl -> Print(This,varAnything)

#define IWorld_Format(This,bstrIn,pObjects,pPOV,bstrOut)	\
    (This)->lpVtbl -> Format(This,bstrIn,pObjects,pPOV,bstrOut)

#define IWorld_FormatSentences(This,bstrIn,pObjects,pPOV,bstrOut)	\
    (This)->lpVtbl -> FormatSentences(This,bstrIn,pObjects,pPOV,bstrOut)

#define IWorld_get_IsConnected(This,pbool)	\
    (This)->lpVtbl -> get_IsConnected(This,pbool)

#define IWorld_get_ServerSide(This,pbool)	\
    (This)->lpVtbl -> get_ServerSide(This,pbool)

#define IWorld_put_ServerSide(This,boolval)	\
    (This)->lpVtbl -> put_ServerSide(This,boolval)

#define IWorld_get_Avatar(This,bstrName,ppthing)	\
    (This)->lpVtbl -> get_Avatar(This,bstrName,ppthing)

#define IWorld_get_AvatarByGUID(This,bstrName,ppthing)	\
    (This)->lpVtbl -> get_AvatarByGUID(This,bstrName,ppthing)

#define IWorld_get_Global(This,ppthing)	\
    (This)->lpVtbl -> get_Global(This,ppthing)

#define IWorld_get_Owner(This,ppthing)	\
    (This)->lpVtbl -> get_Owner(This,ppthing)

#define IWorld_get_User(This,ppthing)	\
    (This)->lpVtbl -> get_User(This,ppthing)

#define IWorld_put_User(This,pthing)	\
    (This)->lpVtbl -> put_User(This,pthing)

#define IWorld_get_This(This,ppthing)	\
    (This)->lpVtbl -> get_This(This,ppthing)

#define IWorld_put_This(This,pthing)	\
    (This)->lpVtbl -> put_This(This,pthing)

#define IWorld_get_Caller(This,ppthing)	\
    (This)->lpVtbl -> get_Caller(This,ppthing)

#define IWorld_put_Caller(This,pthing)	\
    (This)->lpVtbl -> put_Caller(This,pthing)

#define IWorld_get_Super(This,ppthing)	\
    (This)->lpVtbl -> get_Super(This,ppthing)

#define IWorld_put_Super(This,pthing)	\
    (This)->lpVtbl -> put_Super(This,pthing)

#define IWorld_get_SecurityContext(This,ppSecurity)	\
    (This)->lpVtbl -> get_SecurityContext(This,ppSecurity)

#define IWorld_put_SecurityContext(This,pSecurity)	\
    (This)->lpVtbl -> put_SecurityContext(This,pSecurity)

#define IWorld_get_IsInPropChange(This,pbool)	\
    (This)->lpVtbl -> get_IsInPropChange(This,pbool)

#define IWorld_put_IsInPropChange(This,boolval)	\
    (This)->lpVtbl -> put_IsInPropChange(This,boolval)

#define IWorld_AbortEvent(This)	\
    (This)->lpVtbl -> AbortEvent(This)

#define IWorld_get_IsEventAborted(This,pbool)	\
    (This)->lpVtbl -> get_IsEventAborted(This,pbool)

#define IWorld_put_IsEventAborted(This,boolval)	\
    (This)->lpVtbl -> put_IsEventAborted(This,boolval)

#define IWorld_IsExemplar(This,pthing,pbool)	\
    (This)->lpVtbl -> IsExemplar(This,pthing,pbool)

#define IWorld_IsAvatar(This,pthing,pbool)	\
    (This)->lpVtbl -> IsAvatar(This,pthing,pbool)

#define IWorld_IsRoom(This,pthing,pbool)	\
    (This)->lpVtbl -> IsRoom(This,pthing,pbool)

#define IWorld_LockWorld(This)	\
    (This)->lpVtbl -> LockWorld(This)

#define IWorld_UnlockWorld(This)	\
    (This)->lpVtbl -> UnlockWorld(This)

#define IWorld_NotifyUserConnect(This,pthing)	\
    (This)->lpVtbl -> NotifyUserConnect(This,pthing)

#define IWorld_NotifyUserDisconnect(This,pthing)	\
    (This)->lpVtbl -> NotifyUserDisconnect(This,pthing)

#define IWorld_FireUIEvent(This,pthing,bstrEventName,varArg)	\
    (This)->lpVtbl -> FireUIEvent(This,pthing,bstrEventName,varArg)

#define IWorld_ShowHTML(This,bstrTitle,bstrHtml,cx,cy)	\
    (This)->lpVtbl -> ShowHTML(This,bstrTitle,bstrHtml,cx,cy)

#define IWorld_ShowHTMLEx(This,bstrWinName,bstrTitle,bstrHtml,dispArg,x,y,cx,cy)	\
    (This)->lpVtbl -> ShowHTMLEx(This,bstrWinName,bstrTitle,bstrHtml,dispArg,x,y,cx,cy)

#define IWorld_ShowHTMLExt(This,bstrWinName,bstrHtml,bstrTitle,bSafeMode,pdispArg,x,y,cx,cy)	\
    (This)->lpVtbl -> ShowHTMLExt(This,bstrWinName,bstrHtml,bstrTitle,bSafeMode,pdispArg,x,y,cx,cy)

#define IWorld_ShowURL(This,bstrTitle,bstrUrl,cx,cy)	\
    (This)->lpVtbl -> ShowURL(This,bstrTitle,bstrUrl,cx,cy)

#define IWorld_ShowURLEx(This,bstrWinName,bstrTitle,bstrUrl,dispArg,x,y,cx,cy)	\
    (This)->lpVtbl -> ShowURLEx(This,bstrWinName,bstrTitle,bstrUrl,dispArg,x,y,cx,cy)

#define IWorld_ShowURLExt(This,bstrWinName,bPopup,bstrUrl,bstrTitle,bSafeMode,pdispArg,x,y,cx,cy)	\
    (This)->lpVtbl -> ShowURLExt(This,bstrWinName,bPopup,bstrUrl,bstrTitle,bSafeMode,pdispArg,x,y,cx,cy)

#define IWorld_NotifyInvokeMethod(This,pthing,bstr,pdispparams)	\
    (This)->lpVtbl -> NotifyInvokeMethod(This,pthing,bstr,pdispparams)

#define IWorld_NotifyPropertyChanged(This,pthing,bstr,lHint,varHintData)	\
    (This)->lpVtbl -> NotifyPropertyChanged(This,pthing,bstr,lHint,varHintData)

#define IWorld_NotifyMethodChanged(This,pthing,bstr,lHint,varHintData)	\
    (This)->lpVtbl -> NotifyMethodChanged(This,pthing,bstr,lHint,varHintData)

#define IWorld_NotifyModuleChanged(This,bstr,lHint,varHintData)	\
    (This)->lpVtbl -> NotifyModuleChanged(This,bstr,lHint,varHintData)

#define IWorld_SendContainer(This,lval,pthing)	\
    (This)->lpVtbl -> SendContainer(This,lval,pthing)

#define IWorld_SendContents(This,lval,pthing,bForceOpen)	\
    (This)->lpVtbl -> SendContents(This,lval,pthing,bForceOpen)

#define IWorld_SendWorld(This,lval)	\
    (This)->lpVtbl -> SendWorld(This,lval)

#define IWorld_SendExemplar(This,lval,pthing,bstr)	\
    (This)->lpVtbl -> SendExemplar(This,lval,pthing,bstr)

#define IWorld_SendThingByID(This,lval,lID)	\
    (This)->lpVtbl -> SendThingByID(This,lval,lID)

#define IWorld_SendThing(This,lval,pthing)	\
    (This)->lpVtbl -> SendThing(This,lval,pthing)

#define IWorld_SendModule(This,lval,pmodule,bstr)	\
    (This)->lpVtbl -> SendModule(This,lval,pmodule,bstr)

#define IWorld_SendWorldCommand(This,bReply,dispid,wFlags,pdispparams,recvhr,pvar)	\
    (This)->lpVtbl -> SendWorldCommand(This,bReply,dispid,wFlags,pdispparams,recvhr,pvar)

#define IWorld_SendThingCommand(This,lFlags,pthing,dispid,wFlags,pdispparams)	\
    (This)->lpVtbl -> SendThingCommand(This,lFlags,pthing,dispid,wFlags,pdispparams)

#define IWorld_SendObjectPropertyCommand(This,lFlags,pprop,dispid,wFlags,pdispparams)	\
    (This)->lpVtbl -> SendObjectPropertyCommand(This,lFlags,pprop,dispid,wFlags,pdispparams)

#define IWorld_SendWorldCommandExt(This,bReply,bSync,dispid,wFlags,pdispparams,recvhr,pvar)	\
    (This)->lpVtbl -> SendWorldCommandExt(This,bReply,bSync,dispid,wFlags,pdispparams,recvhr,pvar)

#define IWorld_SendThingCommandExt(This,bReply,bSync,lFlags,pthing,dispid,wFlags,pdispparams,recvhr,pvar)	\
    (This)->lpVtbl -> SendThingCommandExt(This,bReply,bSync,lFlags,pthing,dispid,wFlags,pdispparams,recvhr,pvar)

#define IWorld_SendObjectPropertyCommandExt(This,bReply,bSync,lFlags,pprop,dispid,wFlags,pdispparams,recvhr,pvar)	\
    (This)->lpVtbl -> SendObjectPropertyCommandExt(This,bReply,bSync,lFlags,pprop,dispid,wFlags,pdispparams,recvhr,pvar)

#define IWorld_SendModuleCommand(This,pmodule,dispid,wFlags,pdispparams)	\
    (This)->lpVtbl -> SendModuleCommand(This,pmodule,dispid,wFlags,pdispparams)

#define IWorld_SendMethodCommand(This,pmethod,dispid,wFlags,pdispparams)	\
    (This)->lpVtbl -> SendMethodCommand(This,pmethod,dispid,wFlags,pdispparams)

#define IWorld_LogThingCommand(This,pthing,dispid,wFlags,pdispparams)	\
    (This)->lpVtbl -> LogThingCommand(This,pthing,dispid,wFlags,pdispparams)

#define IWorld_LogWorldCommand(This,dispid,wFlags,pdispparams)	\
    (This)->lpVtbl -> LogWorldCommand(This,dispid,wFlags,pdispparams)

#define IWorld_WriteCommandHeader(This,pBuffer,ucReply,ucCommand)	\
    (This)->lpVtbl -> WriteCommandHeader(This,pBuffer,ucReply,ucCommand)

#define IWorld_WriteCommandCallstack(This,pBuffer)	\
    (This)->lpVtbl -> WriteCommandCallstack(This,pBuffer)

#define IWorld_RegisterInterest(This,pthing)	\
    (This)->lpVtbl -> RegisterInterest(This,pthing)

#define IWorld_UnregisterInterest(This,pthing)	\
    (This)->lpVtbl -> UnregisterInterest(This,pthing)

#define IWorld_MarshallThing(This,pthing,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> MarshallThing(This,pthing,dwOptions,pmarshallbuffer)

#define IWorld_UnMarshallThing(This,punmarshallbuffer,ppthing)	\
    (This)->lpVtbl -> UnMarshallThing(This,punmarshallbuffer,ppthing)

#define IWorld_MarshallProperty(This,var,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> MarshallProperty(This,var,dwOptions,pmarshallbuffer)

#define IWorld_UnMarshallProperty(This,punmarshallbuffer,pvar)	\
    (This)->lpVtbl -> UnMarshallProperty(This,punmarshallbuffer,pvar)

#define IWorld_MarshallObjectProperty(This,pprop,lHint,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> MarshallObjectProperty(This,pprop,lHint,dwOptions,pmarshallbuffer)

#define IWorld_UnMarshallObjectProperty(This,punmarshallbuffer,ppprop)	\
    (This)->lpVtbl -> UnMarshallObjectProperty(This,punmarshallbuffer,ppprop)

#define IWorld_MarshallModule(This,pmodule,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> MarshallModule(This,pmodule,dwOptions,pmarshallbuffer)

#define IWorld_UnMarshallModule(This,clsid,punmarshallbuffer,ppmodule)	\
    (This)->lpVtbl -> UnMarshallModule(This,clsid,punmarshallbuffer,ppmodule)

#define IWorld_MarshallDISPPARAMS(This,pdispparams,pmarshallbuffer,dwOptions)	\
    (This)->lpVtbl -> MarshallDISPPARAMS(This,pdispparams,pmarshallbuffer,dwOptions)

#define IWorld_UnMarshallDISPPARAMS(This,punmarshallbuffer,pdispparams)	\
    (This)->lpVtbl -> UnMarshallDISPPARAMS(This,punmarshallbuffer,pdispparams)

#define IWorld_MarshallPropertyToScript(This,var,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallPropertyToScript(This,var,dwOptions,bstrContext,pbAsVal,pbstrOut)

#define IWorld_CheckPolicy(This,pThing,lFlags,var,pbAccessOk)	\
    (This)->lpVtbl -> CheckPolicy(This,pThing,lFlags,var,pbAccessOk)

#define IWorld_CheckPolicyExt(This,pThing,lFlags,var)	\
    (This)->lpVtbl -> CheckPolicyExt(This,pThing,lFlags,var)

#define IWorld_ValidateProperty(This,var)	\
    (This)->lpVtbl -> ValidateProperty(This,var)

#define IWorld_ValidateDISPPARAMS(This,pdispparams)	\
    (This)->lpVtbl -> ValidateDISPPARAMS(This,pdispparams)

#define IWorld_ValidateObjectSafety(This,clsid,punk,dwOptions)	\
    (This)->lpVtbl -> ValidateObjectSafety(This,clsid,punk,dwOptions)

#define IWorld_ValidatePropertyName(This,bstrName,pboolVal)	\
    (This)->lpVtbl -> ValidatePropertyName(This,bstrName,pboolVal)

#define IWorld_ValidateMethodName(This,bstrName,psbits,pboolVal)	\
    (This)->lpVtbl -> ValidateMethodName(This,bstrName,psbits,pboolVal)

#define IWorld_ValidateInstanceName(This,bstrName,pboolVal)	\
    (This)->lpVtbl -> ValidateInstanceName(This,bstrName,pboolVal)

#define IWorld_ValidateExemplarType(This,bstrType,pboolVal)	\
    (This)->lpVtbl -> ValidateExemplarType(This,bstrType,pboolVal)

#define IWorld_ValidateWorldName(This,bstrName,pboolVal)	\
    (This)->lpVtbl -> ValidateWorldName(This,bstrName,pboolVal)

#define IWorld_ValidateAvatarName(This,bstrName,pboolVal)	\
    (This)->lpVtbl -> ValidateAvatarName(This,bstrName,pboolVal)

#define IWorld_ValidatePropertyNameExt(This,bstrName)	\
    (This)->lpVtbl -> ValidatePropertyNameExt(This,bstrName)

#define IWorld_ValidateMethodNameExt(This,bstrName,psbits)	\
    (This)->lpVtbl -> ValidateMethodNameExt(This,bstrName,psbits)

#define IWorld_ValidateInstanceNameExt(This,bstrName)	\
    (This)->lpVtbl -> ValidateInstanceNameExt(This,bstrName)

#define IWorld_ValidateExemplarTypeExt(This,bstrType)	\
    (This)->lpVtbl -> ValidateExemplarTypeExt(This,bstrType)

#define IWorld_ValidateWorldNameExt(This,bstrName)	\
    (This)->lpVtbl -> ValidateWorldNameExt(This,bstrName)

#define IWorld_ValidateAvatarNameExt(This,bstrName)	\
    (This)->lpVtbl -> ValidateAvatarNameExt(This,bstrName)

#define IWorld_CloneProperty(This,source,pDest)	\
    (This)->lpVtbl -> CloneProperty(This,source,pDest)

#define IWorld_DumpProperty(This,var)	\
    (This)->lpVtbl -> DumpProperty(This,var)

#define IWorld_get_Version(This,pbstr)	\
    (This)->lpVtbl -> get_Version(This,pbstr)

#define IWorld_GetStatus(This,pbstr)	\
    (This)->lpVtbl -> GetStatus(This,pbstr)

#define IWorld_AddCLSIDToTable(This,clsid)	\
    (This)->lpVtbl -> AddCLSIDToTable(This,clsid)

#define IWorld_UpdateGlobalPaths(This)	\
    (This)->lpVtbl -> UpdateGlobalPaths(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_Connect_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrPassword,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_Connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_Disconnect_Proxy( 
    IWorld __RPC_FAR * This);


void __RPC_STUB IWorld_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_CreateAvatar_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrPassword,
    /* [optional][in] */ VARIANT bstrGUID,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrNewName);


void __RPC_STUB IWorld_CreateAvatar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_ConnectInt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrPassword,
    /* [in] */ VARIANT varProfile,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_ConnectInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_DisconnectInt_Proxy( 
    IWorld __RPC_FAR * This);


void __RPC_STUB IWorld_DisconnectInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_DisconnectAvatar_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pAvatar);


void __RPC_STUB IWorld_DisconnectAvatar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_DisconnectClient_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pAvatar);


void __RPC_STUB IWorld_DisconnectClient_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_Reconnect_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrURL,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrPassword);


void __RPC_STUB IWorld_Reconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_ReconnectExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrURL,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrPassword);


void __RPC_STUB IWorld_ReconnectExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ActivateRoom_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IWorld_ActivateRoom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_DeactivateRoom_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IWorld_DeactivateRoom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_Synchronize_Proxy( 
    IWorld __RPC_FAR * This);


void __RPC_STUB IWorld_Synchronize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_Database_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IWorld_get_Database_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_OpenDatabase_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstr,
    /* [in] */ VARIANT_BOOL bCreate,
    /* [in] */ VARIANT_BOOL bOverwrite);


void __RPC_STUB IWorld_OpenDatabase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_SaveDatabase_Proxy( 
    IWorld __RPC_FAR * This);


void __RPC_STUB IWorld_SaveDatabase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_SaveDatabaseAs_Proxy( 
    IWorld __RPC_FAR * This,
    BSTR bstrNewName);


void __RPC_STUB IWorld_SaveDatabaseAs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_SaveToScript_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrFilename);


void __RPC_STUB IWorld_SaveToScript_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ProcessLogRecord_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IUnMarshallBuffer __RPC_FAR *pbuffer);


void __RPC_STUB IWorld_ProcessLogRecord_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_EnableLogging_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL boolval);


void __RPC_STUB IWorld_EnableLogging_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWorld_get_IsLoggingEnabled_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_get_IsLoggingEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_CreateObjectInt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lval,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_CreateObjectInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_DeleteObjectInt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IWorld_DeleteObjectInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_CreateInstance_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ VARIANT varExemplar,
    /* [optional][in] */ VARIANT varContainer,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_CreateInstance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_CreateInstanceExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ VARIANT varExemplar,
    /* [in] */ IThing __RPC_FAR *pthingContainer,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_CreateInstanceExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_CreateInstanceInt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IThing __RPC_FAR *pthingExemplar,
    /* [in] */ IThing __RPC_FAR *pthingContainer,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_CreateInstanceInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_CreateExemplar_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrType,
    /* [in] */ VARIANT varExemplar,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_CreateExemplar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_CreateExemplarEx_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrType,
    /* [in] */ VARIANT varExemplar,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_CreateExemplarEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_CreateExemplarExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrType,
    /* [in] */ VARIANT varExemplar,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_CreateExemplarExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_CreateExemplarInt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrType,
    /* [in] */ IThing __RPC_FAR *pthingExemplar,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_CreateExemplarInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_CreateRootExemplar_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrType,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_CreateRootExemplar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_CloneInstance_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IThing __RPC_FAR *pthingSource,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_CloneInstance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_DestroyObject_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IWorld_DestroyObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_DestroyObjectByID_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lval);


void __RPC_STUB IWorld_DestroyObjectByID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_ObjectByName_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstr,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_get_ObjectByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_get_ObjectByID_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lval,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_get_ObjectByID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWorld_get_Exemplar_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrType,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_get_Exemplar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_IsValidExemplar_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrType,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_IsValidExemplar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_get_Exemplars_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap);


void __RPC_STUB IWorld_get_Exemplars_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_CreateObjectProperty_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrProgID,
    /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppproperty);


void __RPC_STUB IWorld_CreateObjectProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_CreateObjectPropertyExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ REFCLSID clsid,
    /* [in] */ IUnknown __RPC_FAR *punk,
    /* [retval][out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppproperty);


void __RPC_STUB IWorld_CreateObjectPropertyExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_CreateMethodByDispID_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lFlags,
    /* [in] */ VARIANT varModule,
    /* [in] */ DISPID dispid,
    /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod);


void __RPC_STUB IWorld_CreateMethodByDispID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_CreateMethod_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lFlags,
    /* [in] */ VARIANT varModule,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod);


void __RPC_STUB IWorld_CreateMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_CreateInlineMethod_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lFlags,
    /* [in] */ BSTR bstrLanguage,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrCode,
    /* [retval][out] */ IMethod __RPC_FAR *__RPC_FAR *ppmethod);


void __RPC_STUB IWorld_CreateInlineMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_CreateCOMModule_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrProgID,
    /* [in] */ long lFlags,
    /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule);


void __RPC_STUB IWorld_CreateCOMModule_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_CreateScriptModule_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrScriptPath,
    /* [in] */ long lFlags,
    /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule);


void __RPC_STUB IWorld_CreateScriptModule_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_CreateCOMModuleEx_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrProgID,
    /* [in] */ long lFlags,
    /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule);


void __RPC_STUB IWorld_CreateCOMModuleEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_CreateScriptModuleEx_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrScriptPath,
    /* [in] */ long lFlags,
    /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule);


void __RPC_STUB IWorld_CreateScriptModuleEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_RemoveModule_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IWorld_RemoveModule_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWorld_get_Module_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule);


void __RPC_STUB IWorld_get_Module_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_IsValidModule_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_IsValidModule_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_get_Modules_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropmap);


void __RPC_STUB IWorld_get_Modules_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_CreateTool_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrProgID,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);


void __RPC_STUB IWorld_CreateTool_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_CreateToolFromInterface_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IUnknown __RPC_FAR *punk);


void __RPC_STUB IWorld_CreateToolFromInterface_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_CreateToolExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ REFCLSID clsid,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);


void __RPC_STUB IWorld_CreateToolExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_RemoveTool_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IWorld_RemoveTool_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_IsValidTool_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_IsValidTool_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_get_Tool_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);


void __RPC_STUB IWorld_get_Tool_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_ToolExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);


void __RPC_STUB IWorld_get_ToolExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_Tools_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropmap);


void __RPC_STUB IWorld_get_Tools_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_InitializeTools_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IPropertyMap __RPC_FAR *ppropmap);


void __RPC_STUB IWorld_InitializeTools_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_InitializeConnection_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IVWCommConnection __RPC_FAR *pvwcommconnection);


void __RPC_STUB IWorld_InitializeConnection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_DisconnectConnection_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IVWCommConnection __RPC_FAR *pvwcommconnection);


void __RPC_STUB IWorld_DisconnectConnection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_OnOpenWorld_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bPersist);


void __RPC_STUB IWorld_OnOpenWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_OnCreateWorld_Proxy( 
    IWorld __RPC_FAR * This);


void __RPC_STUB IWorld_OnCreateWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ProcessMessage_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IVWCommMessage __RPC_FAR *pmessage);


void __RPC_STUB IWorld_ProcessMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ProcessMessageExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ unsigned char ch,
    /* [in] */ IUnMarshallBuffer __RPC_FAR *pUBuffer,
    /* [out] */ HRESULT __RPC_FAR *recvhr,
    /* [out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IWorld_ProcessMessageExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_Connection_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ IVWCommConnection __RPC_FAR *__RPC_FAR *ppvwcommconnection);


void __RPC_STUB IWorld_get_Connection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_put_Connection_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IVWCommConnection __RPC_FAR *pvwcommconnection);


void __RPC_STUB IWorld_put_Connection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_ShouldExecute_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_get_ShouldExecute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_ShouldExecuteSync_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_get_ShouldExecuteSync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_ShouldRemote_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_get_ShouldRemote_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_ShouldRemoteSync_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_get_ShouldRemoteSync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_ShouldNotify_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_get_ShouldNotify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_LocalMode_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_get_LocalMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_put_LocalMode_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL boolval);


void __RPC_STUB IWorld_put_LocalMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_FreezeEvents_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_get_FreezeEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_put_FreezeEvents_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL boolval);


void __RPC_STUB IWorld_put_FreezeEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWorld_get_EnableSecurity_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_get_EnableSecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_EnableTrace_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL boolval);


void __RPC_STUB IWorld_EnableTrace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_get_IsTraceEnabled_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_get_IsTraceEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_get_TraceLevel_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstr,
    /* [retval][out] */ int __RPC_FAR *ival);


void __RPC_STUB IWorld_get_TraceLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_put_TraceLevel_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstr,
    /* [in] */ int ival);


void __RPC_STUB IWorld_put_TraceLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_RemoveTraceLevel_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IWorld_RemoveTraceLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_Trace_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IWorld_Trace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_Report_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstr,
    /* [in] */ long lType);


void __RPC_STUB IWorld_Report_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_Print_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT varAnything);


void __RPC_STUB IWorld_Print_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_Format_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrIn,
    /* [in] */ IPropertyList __RPC_FAR *pObjects,
    /* [in] */ IThing __RPC_FAR *pPOV,
    /* [retval][out] */ BSTR __RPC_FAR *bstrOut);


void __RPC_STUB IWorld_Format_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_FormatSentences_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrIn,
    /* [in] */ IPropertyList __RPC_FAR *pObjects,
    /* [in] */ IThing __RPC_FAR *pPOV,
    /* [retval][out] */ BSTR __RPC_FAR *bstrOut);


void __RPC_STUB IWorld_FormatSentences_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_IsConnected_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_get_IsConnected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWorld_get_ServerSide_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_get_ServerSide_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_put_ServerSide_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL boolval);


void __RPC_STUB IWorld_put_ServerSide_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_Avatar_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_get_Avatar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_AvatarByGUID_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_get_AvatarByGUID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWorld_get_Global_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_get_Global_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWorld_get_Owner_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_get_Owner_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWorld_get_User_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_get_User_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_put_User_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IWorld_put_User_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWorld_get_This_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_get_This_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_put_This_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IWorld_put_This_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWorld_get_Caller_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_get_Caller_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_put_Caller_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IWorld_put_Caller_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_Super_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_get_Super_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_put_Super_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IWorld_put_Super_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_SecurityContext_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);


void __RPC_STUB IWorld_get_SecurityContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_put_SecurityContext_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);


void __RPC_STUB IWorld_put_SecurityContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_IsInPropChange_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_get_IsInPropChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_put_IsInPropChange_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL boolval);


void __RPC_STUB IWorld_put_IsInPropChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_AbortEvent_Proxy( 
    IWorld __RPC_FAR * This);


void __RPC_STUB IWorld_AbortEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_get_IsEventAborted_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_get_IsEventAborted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_put_IsEventAborted_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL boolval);


void __RPC_STUB IWorld_put_IsEventAborted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_IsExemplar_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_IsExemplar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_IsAvatar_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_IsAvatar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_IsRoom_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWorld_IsRoom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_LockWorld_Proxy( 
    IWorld __RPC_FAR * This);


void __RPC_STUB IWorld_LockWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_UnlockWorld_Proxy( 
    IWorld __RPC_FAR * This);


void __RPC_STUB IWorld_UnlockWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_NotifyUserConnect_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IWorld_NotifyUserConnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_NotifyUserDisconnect_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IWorld_NotifyUserDisconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_FireUIEvent_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrEventName,
    /* [optional][in] */ VARIANT varArg);


void __RPC_STUB IWorld_FireUIEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_ShowHTML_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ BSTR bstrHtml,
    /* [defaultvalue][optional][in] */ VARIANT cx,
    /* [defaultvalue][optional][in] */ VARIANT cy);


void __RPC_STUB IWorld_ShowHTML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_ShowHTMLEx_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrWinName,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ BSTR bstrHtml,
    /* [in] */ VARIANT dispArg,
    /* [defaultvalue][optional][in] */ VARIANT x,
    /* [defaultvalue][optional][in] */ VARIANT y,
    /* [defaultvalue][optional][in] */ VARIANT cx,
    /* [defaultvalue][optional][in] */ VARIANT cy);


void __RPC_STUB IWorld_ShowHTMLEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ShowHTMLExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrWinName,
    /* [in] */ BSTR bstrHtml,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ VARIANT_BOOL bSafeMode,
    /* [in] */ IDispatch __RPC_FAR *pdispArg,
    /* [in] */ int x,
    /* [in] */ int y,
    /* [in] */ int cx,
    /* [in] */ int cy);


void __RPC_STUB IWorld_ShowHTMLExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_ShowURL_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ BSTR bstrUrl,
    /* [defaultvalue][optional][in] */ VARIANT cx,
    /* [defaultvalue][optional][in] */ VARIANT cy);


void __RPC_STUB IWorld_ShowURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_ShowURLEx_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrWinName,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ BSTR bstrUrl,
    /* [in] */ VARIANT dispArg,
    /* [defaultvalue][optional][in] */ VARIANT x,
    /* [defaultvalue][optional][in] */ VARIANT y,
    /* [defaultvalue][optional][in] */ VARIANT cx,
    /* [defaultvalue][optional][in] */ VARIANT cy);


void __RPC_STUB IWorld_ShowURLEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ShowURLExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrWinName,
    /* [in] */ VARIANT_BOOL bPopup,
    /* [in] */ BSTR bstrUrl,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ VARIANT_BOOL bSafeMode,
    /* [in] */ IDispatch __RPC_FAR *pdispArg,
    /* [in] */ int x,
    /* [in] */ int y,
    /* [in] */ int cx,
    /* [in] */ int cy);


void __RPC_STUB IWorld_ShowURLExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_NotifyInvokeMethod_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstr,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);


void __RPC_STUB IWorld_NotifyInvokeMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_NotifyPropertyChanged_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstr,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData);


void __RPC_STUB IWorld_NotifyPropertyChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_NotifyMethodChanged_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstr,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData);


void __RPC_STUB IWorld_NotifyMethodChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_NotifyModuleChanged_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstr,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData);


void __RPC_STUB IWorld_NotifyModuleChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendContainer_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lval,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IWorld_SendContainer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendContents_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lval,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ VARIANT_BOOL bForceOpen);


void __RPC_STUB IWorld_SendContents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendWorld_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lval);


void __RPC_STUB IWorld_SendWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendExemplar_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lval,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstr);


void __RPC_STUB IWorld_SendExemplar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendThingByID_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lval,
    /* [in] */ long lID);


void __RPC_STUB IWorld_SendThingByID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendThing_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lval,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IWorld_SendThing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendModule_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lval,
    /* [in] */ IModule __RPC_FAR *pmodule,
    /* [in] */ BSTR bstr);


void __RPC_STUB IWorld_SendModule_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendWorldCommand_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bReply,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [out] */ HRESULT __RPC_FAR *recvhr,
    /* [out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IWorld_SendWorldCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendThingCommand_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lFlags,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);


void __RPC_STUB IWorld_SendThingCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendObjectPropertyCommand_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ long lFlags,
    /* [in] */ IObjectProperty __RPC_FAR *pprop,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);


void __RPC_STUB IWorld_SendObjectPropertyCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendWorldCommandExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bReply,
    /* [in] */ VARIANT_BOOL bSync,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [out] */ HRESULT __RPC_FAR *recvhr,
    /* [out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IWorld_SendWorldCommandExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendThingCommandExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bReply,
    /* [in] */ VARIANT_BOOL bSync,
    /* [in] */ long lFlags,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [out] */ HRESULT __RPC_FAR *recvhr,
    /* [out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IWorld_SendThingCommandExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendObjectPropertyCommandExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bReply,
    /* [in] */ VARIANT_BOOL bSync,
    /* [in] */ long lFlags,
    /* [in] */ IObjectProperty __RPC_FAR *pprop,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [out] */ HRESULT __RPC_FAR *recvhr,
    /* [out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IWorld_SendObjectPropertyCommandExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendModuleCommand_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IModule __RPC_FAR *pmodule,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);


void __RPC_STUB IWorld_SendModuleCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_SendMethodCommand_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IMethod __RPC_FAR *pmethod,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);


void __RPC_STUB IWorld_SendMethodCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_LogThingCommand_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);


void __RPC_STUB IWorld_LogThingCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_LogWorldCommand_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);


void __RPC_STUB IWorld_LogWorldCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_WriteCommandHeader_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IMarshallBuffer __RPC_FAR *pBuffer,
    /* [in] */ unsigned char ucReply,
    /* [in] */ unsigned char ucCommand);


void __RPC_STUB IWorld_WriteCommandHeader_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_WriteCommandCallstack_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IMarshallBuffer __RPC_FAR *pBuffer);


void __RPC_STUB IWorld_WriteCommandCallstack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_RegisterInterest_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IWorld_RegisterInterest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_UnregisterInterest_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IWorld_UnregisterInterest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_MarshallThing_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ DWORD dwOptions,
    /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);


void __RPC_STUB IWorld_MarshallThing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_UnMarshallThing_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
    /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IWorld_UnMarshallThing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_MarshallProperty_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT var,
    /* [in] */ DWORD dwOptions,
    /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);


void __RPC_STUB IWorld_MarshallProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_UnMarshallProperty_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
    /* [out] */ VARIANT __RPC_FAR *pvar);


void __RPC_STUB IWorld_UnMarshallProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_MarshallObjectProperty_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IObjectProperty __RPC_FAR *pprop,
    /* [in] */ long lHint,
    /* [in] */ DWORD dwOptions,
    /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);


void __RPC_STUB IWorld_MarshallObjectProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_UnMarshallObjectProperty_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
    /* [out] */ IObjectProperty __RPC_FAR *__RPC_FAR *ppprop);


void __RPC_STUB IWorld_UnMarshallObjectProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_MarshallModule_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IModule __RPC_FAR *pmodule,
    /* [in] */ DWORD dwOptions,
    /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);


void __RPC_STUB IWorld_MarshallModule_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_UnMarshallModule_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ REFCLSID clsid,
    /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
    /* [out] */ IModule __RPC_FAR *__RPC_FAR *ppmodule);


void __RPC_STUB IWorld_UnMarshallModule_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_MarshallDISPPARAMS_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer,
    /* [in] */ DWORD dwOptions);


void __RPC_STUB IWorld_MarshallDISPPARAMS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_UnMarshallDISPPARAMS_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer,
    /* [out][in] */ DISPPARAMS __RPC_FAR *pdispparams);


void __RPC_STUB IWorld_UnMarshallDISPPARAMS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_MarshallPropertyToScript_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT var,
    /* [in] */ DWORD dwOptions,
    /* [in] */ BSTR bstrContext,
    /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
    /* [out] */ BSTR __RPC_FAR *pbstrOut);


void __RPC_STUB IWorld_MarshallPropertyToScript_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWorld_CheckPolicy_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ unsigned long lFlags,
    /* [optional][in] */ VARIANT var,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAccessOk);


void __RPC_STUB IWorld_CheckPolicy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_CheckPolicyExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ unsigned long lFlags,
    /* [optional][in] */ VARIANT var);


void __RPC_STUB IWorld_CheckPolicyExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ValidateProperty_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT var);


void __RPC_STUB IWorld_ValidateProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ValidateDISPPARAMS_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);


void __RPC_STUB IWorld_ValidateDISPPARAMS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ValidateObjectSafety_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ REFCLSID clsid,
    /* [in] */ IUnknown __RPC_FAR *punk,
    /* [in] */ DWORD dwOptions);


void __RPC_STUB IWorld_ValidateObjectSafety_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_ValidatePropertyName_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IWorld_ValidatePropertyName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_ValidateMethodName_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ long psbits,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IWorld_ValidateMethodName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_ValidateInstanceName_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IWorld_ValidateInstanceName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_ValidateExemplarType_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrType,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IWorld_ValidateExemplarType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_ValidateWorldName_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IWorld_ValidateWorldName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_ValidateAvatarName_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IWorld_ValidateAvatarName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ValidatePropertyNameExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IWorld_ValidatePropertyNameExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ValidateMethodNameExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ long psbits);


void __RPC_STUB IWorld_ValidateMethodNameExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ValidateInstanceNameExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IWorld_ValidateInstanceNameExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ValidateExemplarTypeExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrType);


void __RPC_STUB IWorld_ValidateExemplarTypeExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ValidateWorldNameExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IWorld_ValidateWorldNameExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_ValidateAvatarNameExt_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IWorld_ValidateAvatarNameExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_CloneProperty_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT source,
    /* [retval][out] */ VARIANT __RPC_FAR *pDest);


void __RPC_STUB IWorld_CloneProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_DumpProperty_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ VARIANT var);


void __RPC_STUB IWorld_DumpProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_get_Version_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IWorld_get_Version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_GetStatus_Proxy( 
    IWorld __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IWorld_GetStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IWorld_AddCLSIDToTable_Proxy( 
    IWorld __RPC_FAR * This,
    /* [in] */ REFCLSID clsid);


void __RPC_STUB IWorld_AddCLSIDToTable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWorld_UpdateGlobalPaths_Proxy( 
    IWorld __RPC_FAR * This);


void __RPC_STUB IWorld_UpdateGlobalPaths_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWorld_INTERFACE_DEFINED__ */


#ifndef __IVWObjectSite_INTERFACE_DEFINED__
#define __IVWObjectSite_INTERFACE_DEFINED__

/* interface IVWObjectSite */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IVWObjectSite;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D9856-A417-11CF-9BF0-0080C7A56A8A")
    IVWObjectSite : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnUserConnect( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnUserDisconnect( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnUserReconnect( 
            BSTR bstrURL,
            BSTR bstrName,
            BSTR bstrPassword) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnUIEvent( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrEventName,
            /* [in] */ VARIANT varArg) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnInvokeMethod( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrName,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnPropertyChanged( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrName,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnMethodChanged( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrName,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnModuleChanged( 
            /* [in] */ BSTR bstrName,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnTrace( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnReport( 
            /* [in] */ BSTR bstr,
            /* [in] */ long lType) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWObjectSiteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWObjectSite __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWObjectSite __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWObjectSite __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUserConnect )( 
            IVWObjectSite __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUserDisconnect )( 
            IVWObjectSite __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUserReconnect )( 
            IVWObjectSite __RPC_FAR * This,
            BSTR bstrURL,
            BSTR bstrName,
            BSTR bstrPassword);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUIEvent )( 
            IVWObjectSite __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrEventName,
            /* [in] */ VARIANT varArg);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnInvokeMethod )( 
            IVWObjectSite __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrName,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnPropertyChanged )( 
            IVWObjectSite __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrName,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnMethodChanged )( 
            IVWObjectSite __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrName,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnModuleChanged )( 
            IVWObjectSite __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnTrace )( 
            IVWObjectSite __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnReport )( 
            IVWObjectSite __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ long lType);
        
        END_INTERFACE
    } IVWObjectSiteVtbl;

    interface IVWObjectSite
    {
        CONST_VTBL struct IVWObjectSiteVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWObjectSite_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWObjectSite_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWObjectSite_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWObjectSite_OnUserConnect(This,pthing)	\
    (This)->lpVtbl -> OnUserConnect(This,pthing)

#define IVWObjectSite_OnUserDisconnect(This,pthing)	\
    (This)->lpVtbl -> OnUserDisconnect(This,pthing)

#define IVWObjectSite_OnUserReconnect(This,bstrURL,bstrName,bstrPassword)	\
    (This)->lpVtbl -> OnUserReconnect(This,bstrURL,bstrName,bstrPassword)

#define IVWObjectSite_OnUIEvent(This,pthing,bstrEventName,varArg)	\
    (This)->lpVtbl -> OnUIEvent(This,pthing,bstrEventName,varArg)

#define IVWObjectSite_OnInvokeMethod(This,pthing,bstrName,pdispparams)	\
    (This)->lpVtbl -> OnInvokeMethod(This,pthing,bstrName,pdispparams)

#define IVWObjectSite_OnPropertyChanged(This,pthing,bstrName,lHint,varHintData)	\
    (This)->lpVtbl -> OnPropertyChanged(This,pthing,bstrName,lHint,varHintData)

#define IVWObjectSite_OnMethodChanged(This,pthing,bstrName,lHint,varHintData)	\
    (This)->lpVtbl -> OnMethodChanged(This,pthing,bstrName,lHint,varHintData)

#define IVWObjectSite_OnModuleChanged(This,bstrName,lHint,varHintData)	\
    (This)->lpVtbl -> OnModuleChanged(This,bstrName,lHint,varHintData)

#define IVWObjectSite_OnTrace(This,bstr)	\
    (This)->lpVtbl -> OnTrace(This,bstr)

#define IVWObjectSite_OnReport(This,bstr,lType)	\
    (This)->lpVtbl -> OnReport(This,bstr,lType)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectSite_OnUserConnect_Proxy( 
    IVWObjectSite __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IVWObjectSite_OnUserConnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectSite_OnUserDisconnect_Proxy( 
    IVWObjectSite __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IVWObjectSite_OnUserDisconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectSite_OnUserReconnect_Proxy( 
    IVWObjectSite __RPC_FAR * This,
    BSTR bstrURL,
    BSTR bstrName,
    BSTR bstrPassword);


void __RPC_STUB IVWObjectSite_OnUserReconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectSite_OnUIEvent_Proxy( 
    IVWObjectSite __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrEventName,
    /* [in] */ VARIANT varArg);


void __RPC_STUB IVWObjectSite_OnUIEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectSite_OnInvokeMethod_Proxy( 
    IVWObjectSite __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrName,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);


void __RPC_STUB IVWObjectSite_OnInvokeMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectSite_OnPropertyChanged_Proxy( 
    IVWObjectSite __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrName,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData);


void __RPC_STUB IVWObjectSite_OnPropertyChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectSite_OnMethodChanged_Proxy( 
    IVWObjectSite __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrName,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData);


void __RPC_STUB IVWObjectSite_OnMethodChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectSite_OnModuleChanged_Proxy( 
    IVWObjectSite __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData);


void __RPC_STUB IVWObjectSite_OnModuleChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectSite_OnTrace_Proxy( 
    IVWObjectSite __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IVWObjectSite_OnTrace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectSite_OnReport_Proxy( 
    IVWObjectSite __RPC_FAR * This,
    /* [in] */ BSTR bstr,
    /* [in] */ long lType);


void __RPC_STUB IVWObjectSite_OnReport_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWObjectSite_INTERFACE_DEFINED__ */


#ifndef __IVWDispatch_INTERFACE_DEFINED__
#define __IVWDispatch_INTERFACE_DEFINED__

/* interface IVWDispatch */
/* [unique][helpstring][dual][uuid][object] */ 

#define	DISPID_VWDISPATCH_INSTALL	( 1000 )

#define	DISPID_VWDISPATCH_ONLOAD	( 1001 )

#define	DISPID_VWDISPATCH_ONUNLOAD	( 1002 )


EXTERN_C const IID IID_IVWDispatch;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D9858-A417-11CF-9BF0-0080C7A56A8A")
    IVWDispatch : public IDispatch
    {
    public:
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ IWorld __RPC_FAR *pworld) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Terminate( void) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Install( 
            /* [in] */ IModule __RPC_FAR *pmodule) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE OnLoad( void) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE OnUnload( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWDispatchVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWDispatch __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWDispatch __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWDispatch __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IVWDispatch __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IVWDispatch __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IVWDispatch __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IVWDispatch __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IVWDispatch __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IVWDispatch __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Install )( 
            IVWDispatch __RPC_FAR * This,
            /* [in] */ IModule __RPC_FAR *pmodule);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLoad )( 
            IVWDispatch __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUnload )( 
            IVWDispatch __RPC_FAR * This);
        
        END_INTERFACE
    } IVWDispatchVtbl;

    interface IVWDispatch
    {
        CONST_VTBL struct IVWDispatchVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWDispatch_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWDispatch_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWDispatch_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWDispatch_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IVWDispatch_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IVWDispatch_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IVWDispatch_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IVWDispatch_Initialize(This,pworld)	\
    (This)->lpVtbl -> Initialize(This,pworld)

#define IVWDispatch_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IVWDispatch_Install(This,pmodule)	\
    (This)->lpVtbl -> Install(This,pmodule)

#define IVWDispatch_OnLoad(This)	\
    (This)->lpVtbl -> OnLoad(This)

#define IVWDispatch_OnUnload(This)	\
    (This)->lpVtbl -> OnUnload(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWDispatch_Initialize_Proxy( 
    IVWDispatch __RPC_FAR * This,
    /* [in] */ IWorld __RPC_FAR *pworld);


void __RPC_STUB IVWDispatch_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWDispatch_Terminate_Proxy( 
    IVWDispatch __RPC_FAR * This);


void __RPC_STUB IVWDispatch_Terminate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IVWDispatch_Install_Proxy( 
    IVWDispatch __RPC_FAR * This,
    /* [in] */ IModule __RPC_FAR *pmodule);


void __RPC_STUB IVWDispatch_Install_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IVWDispatch_OnLoad_Proxy( 
    IVWDispatch __RPC_FAR * This);


void __RPC_STUB IVWDispatch_OnLoad_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][helpstring] */ HRESULT STDMETHODCALLTYPE IVWDispatch_OnUnload_Proxy( 
    IVWDispatch __RPC_FAR * This);


void __RPC_STUB IVWDispatch_OnUnload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWDispatch_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_vwobject_0245 */
/* [local] */ 

typedef /* [helpstring][uuid] */ 
enum ModuleConstant
    {	mcCOM	= 0,
	mcScript	= 1
    }	ModuleConstant;

typedef /* [helpstring][uuid] */ 
enum SendObjectConstant
    {	soClient	= 0,
	soBystanders	= 1,
	soAvatars	= 2,
	soServer	= 3,
	soBystandersExclUser	= 4,
	soAvatarsExclUser	= 5,
	soAvatar	= 6
    }	SendObjectConstant;

typedef /* [helpstring][uuid] */ 
enum VWTraceLevels
    {	VWT_VERBOSE	= 0,
	VWT_DUMP	= 1,
	VWT_METHOD	= 2,
	VWT_IMPORTANT	= 3,
	VWT_ERROR	= 4,
	VWT_NONE	= 5
    }	VWTraceLevels;

typedef /* [helpstring][uuid] */ 
enum VWReportTypes
    {	VWR_ERROR	= -1,
	VWR_PRINT	= 0,
	VWR_ECHO	= 1,
	VWR_EVAL	= 2,
	VWR_PROMPT	= 3,
	VWR_PERFLOG	= 4,
	VWR_PAGE	= 5
    }	VWReportTypes;

typedef /* [helpstring][uuid] */ 
enum MenuItemHint
    {	miFlags	= 0,
	miUserData	= 1,
	miSubMenu	= 2,
	miStatusText	= 3,
	miMethod	= 4,
	miOnUpdateMethod	= 5,
	miArguments	= 6,
	miOwner	= 7,
	miMenuText	= 8,
	miSortOrder	= 9,
	miTarget	= 10,
	miAddTargetParameter	= 11,
	miIsOverlayItem	= 12,
	miIsProximitySensitive	= 13
    }	MenuItemHint;

typedef /* [helpstring][uuid] */ 
enum MethodHint
    {	meFlags	= 0,
	meModule	= 1,
	meArguments	= 2,
	meDispID	= 3,
	meMethodName	= 4,
	meCode	= 5
    }	MethodHint;

typedef /* [helpstring][uuid] */ 
enum PropertyListHint
    {	plAdd	= 0,
	plInsertAt	= 1,
	plRemove	= 2,
	plRemoveAt	= 3,
	plRemoveAll	= 4,
	plPut	= 5
    }	PropertyListHint;

typedef /* [helpstring][uuid] */ 
enum PropertyMapHint
    {	pmPut	= 0,
	pmRemove	= 1,
	pmRemoveAll	= 2
    }	PropertyMapHint;

typedef /* [helpstring][uuid] */ 
enum ThingHint
    {	thAdd	= 100,
	thPut	= 101,
	thRemove	= 102
    }	ThingHint;

typedef /* [helpstring][uuid] */ 
enum CLSIDHint
    {	clsidNull	= 0,
	clsidPropertyList	= 1,
	clsidPropertyMap	= 2,
	clsidCOMModule	= 3,
	clsidScriptModule	= 4,
	clsidMethod	= 5,
	clsidThing	= 6,
	clsidWorld	= 7,
	clsidMenuItem	= 8,
	clsidOCXProxy	= 9,
	clsidAvatarProfile	= 10,
	clsidAvatarProfileData	= 11,
	clsidUnknown	= 255
    }	CLSIDHint;

typedef /* [helpstring][uuid] */ 
enum ModuleHint
    {	moCreate	= 0,
	moRemove	= 1
    }	ModuleHint;

typedef /* [helpstring][uuid] */ 
enum COMModuleHint
    {	cmName	= 10,
	cmFlags	= 11,
	cmCLSID	= 12,
	cmProgID	= 13
    }	COMModuleHint;

typedef /* [helpstring][uuid] */ 
enum ScriptModuleHint
    {	smName	= 10,
	smFlags	= 11,
	smScriptPath	= 12
    }	ScriptModuleHint;

typedef /* [helpstring][uuid] */ 
enum ObjectPropertyHint
    {	opType	= 50
    }	ObjectPropertyHint;



extern RPC_IF_HANDLE __MIDL_itf_vwobject_0245_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vwobject_0245_v0_0_s_ifspec;

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
