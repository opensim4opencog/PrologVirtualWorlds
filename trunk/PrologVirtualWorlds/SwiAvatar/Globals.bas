Attribute VB_Name = "Globals"
Option Explicit

Public theKnownMorphs As PropertyMap


Public theIAvatar As Object
Public theCOMMorphism As COMMorphism
Public theSWIPrologShell As SWIProlog
Public theWorldMorphism As WorldMorphism
Public theAvatarMorphism As AvatarMorphism
Public theComClassBrowser As fComClassBrowser
Public theUIClientBrowser As fWebBrowser
Public theMorphism As Morphism

Public theHiddenMembers As String
Public theExposedMembers As String
Public theRefMembers As String


Public java_vbscript_engine As msscriptcontrol.ScriptControl

Public theMainForm As fMainMDI

Public thePrologTracer As fPrologTracer


Public DLLFiles As PropertyMap
Public theAddRefs As Collection
Public theTypeNameInterfaces As PropertyMap
Public theLoadedVBSModules As PropertyList
'Public PowerVBTLBEdit As fPowerVBTLBEdit
Public VectoredObjects As String
Public ClassMembers As PropertyMap
Public HumanAvatarName As String
Public HumanAvatarPass As String
Public LoginSucceeded As Boolean
Public cm As CollectionMember

'Public theXMLDocument As MSXML2.FreeThreadedDOMDocument

Public theRootDatum As a_Datum

'Default install path for SWI
Public SWIInstallPath As String


Public SWIWorkPath As String
Public SWILibPath As String
Public SWIExtension As String
Public VWInstallPath As String
Public VWContentPath As String
Public PVWExePath As String
Public PVWInstallPathNow As String
Public PVWInstallPath As String
Public PVWLibraryPath As String
Public PVWProjectPath As String
Public theSoftwareRegKey As RegKey
Public PVWRegKey As RegKey
Public PVWUIClientPath As String

Public LocalVWInstall As String

Public theWorldName As String
Public theAvatarName As String


Public theDatums As IPropertyMap

Public ws As MSWinsockLib.Winsock
'Public david_hovel As MSBN3Lib.MSBN
Public readylogon As Boolean


Public object_data As Boolean
Public textevents_flag As Boolean
Public filterex_flag As Boolean
Public tracelevel As Byte

Public expand_ithings_flag As Boolean

Public savepath As String

Public m_ready_flag As Boolean

Public nl_proc_flag As Boolean

Public comment_delimiter As String
Public comment_opener As String
Public comment_terminator As String

Public list_opener As String
Public list_seperator As String
Public list_terminator As String

Public set_opener As String
Public set_seperator As String
Public set_terminator As String

Public command_seperator As String
Public functionlcase As Boolean
Public argument_opener As String
Public argument_seperator As String
Public argument_terminator As String

Public object_opener As String
Public object_terminator As String
Public objectlcase_flag As Boolean
Public typeobject_flag As Boolean

Public string_opener As String
Public string_terminator As String
Public stringlcase_flag As Boolean
Public stringlist_opener As String
Public stringlist_seperator As String
Public stringlist_terminator As String
Public string_liststrs_flag As Boolean
Public escapable_chars As ipropertylist
Public escapechar As String
'public nl_proc_flag as boolean


