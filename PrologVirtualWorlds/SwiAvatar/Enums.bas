Attribute VB_Name = "EnumsAndConsts"
Option Explicit
'Public Const INVOKE_PROPERTYPUT_INVOKE_PROPERTYGET = (tli.InvokeKinds.INVOKE_PROPERTYPUT Or tli.InvokeKinds.INVOKE_PROPERTYGET)



Public Type ithingwithrating
    thething As IThing
    therating As Single
End Type

Public Enum PrologResult
    Success = 0
    Retry = 1
    Exception = 2
    Failure = 3
End Enum

Public Const vbQt = """"

Const EM_UNDO = &HC7

Public Type DISPPARAMS
             rgvarg As Variant
             rgdispidNameArgs As Long 'dispid
             cArgs As Long
             cNamedArgs As Long
End Type


Public Type PROCESS_INFORMATION
      hProcess As Long
      hThread As Long
      dwProcessID As Long
      dwThreadID As Long
   End Type
   

Public Type STARTUPINFO
      cb As Long
      lpReserved As String
      lpDesktop As String
      lpTitle As String
      dwX As Long
      dwY As Long
      dwXSize As Long
      dwYSize As Long
      dwXCountChars As Long
      dwYCountChars As Long
      dwFillAttribute As Long
      dwFlags As Long
      wShowWindow As Integer
      cbReserved2 As Integer
      lpReserved2 As Long
      hStdInput As Long
      hStdOutput As Long
      hStdError As Long
   End Type

Public Type uGUID
    Data1 As Long
    Data2 As Integer
    Data3 As Integer
    Data4(7) As Byte
End Type


Public Const NORMAL_PRIORITY_CLASS = &H20&
Public Const INFINITE = -1&

Public Const IID_IDispatch = "{00020400-0000-0000-C000-000000000046}"
Public Const IID_IPersistStorage = "{0000010A-0000-0000-C000-000000000046}"
Public Const IID_IPersistStream = "{00000109-0000-0000-C000-000000000046}"
Public Const IID_IPersistPropertyBag = "{37D84F60-42CB-11CE-8135-00AA004BB851}"

Public Const INTERFACESAFE_FOR_UNTRUSTED_CALLER = &H1   ' Caller of interface may be untrusted
Public Const INTERFACESAFE_FOR_UNTRUSTED_DATA = &H2     ' Data passed into interface may be untrusted

Public Const E_NOINTERFACE = &H80004002 ' No such interface supported
Public Const E_FAIL = &H80004005        ' Unspecified error

Public Const MAX_GUIDLEN = 40

Public Const WorkPath = "C:\msr\output\"

Public m_fSafeForScripting      As Boolean
Public m_fSafeForInitializing   As Boolean

Public Const SWP_NOMOVE = 2
Public Const SWP_NOSIZE = 1
Public Const Flags = SWP_NOMOVE Or SWP_NOSIZE
Public Const HWND_TOPMOST = -1
Public Const HWND_NOTOPMOST = -2

Type POINTAPI
        x As Long
        y As Long
End Type

Public Enum QueryLanguages
    NullQuery = 0
    VBScriptQuery = 1
    MSBNQuery = 2
    RAIDQuery = 3
    PLQuery = 4
    MOOQuery = 5
    WebQuery = 6
    JScriptQuery = 7
    XMLQuery = 8
    SQLQuery = 9
    NLPQuery = 10
    SwiBotQuery = 11
    TelnetQuery = 12
    InConcertQuery = 13
    VWorldQuery = 14
    UserQuery = 15
    ScriptQuery = 16
End Enum


Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)


'OnTell event type constants
Public Function tell_enum_string(ByVal inttCtype)
    Select Case inttCtype
        Case 0
                tell_enum_string = "say"
        Case 1
                tell_enum_string = "emote"
        Case 2
                tell_enum_string = "think"
        Case 3
                tell_enum_string = "whisper"
        Case 4
                tell_enum_string = "announce"
        Case 5
                tell_enum_string = "gesture"
        Case 6
                tell_enum_string = "shout"
        Case 7
                tell_enum_string = "report"
        Case 8
                tell_enum_string = "action"
        Case Else
                tell_enum_string = "tellUnknown"
    End Select
End Function

'Other UI Event constants used by this page
Public Function uitype_enum_string(ltype)
    Select Case ltype
        Case 0
            uitype_enum_string = "uiNone"
        Case 1
            uitype_enum_string = "uiLogOff"
        Case 2
            uitype_enum_string = "uiWelcome"
        Case 3
            uitype_enum_string = "uiEvent"
        Case 4
            uitype_enum_string = "uiMUDReport"
        Case Else
            uitype_enum_string = "uiUnknown"
    End Select
End Function


'Report type constants
Public Function int_reporttype(ByVal ltype)
    Select Case ltype
        Case -1
            int_reporttype = "vwErrorText"
        Case 0
            int_reporttype = "vwPrintText"
        Case 1
            int_reporttype = "vwEchoText"
        Case 2
            int_reporttype = "vwEvalText"
        Case 3
            int_reporttype = "vwPrompt"
        Case 4
            int_reporttype = "vwPerfLog"
        Case 5
            int_reporttype = "vwPage"
        Case Else
            int_reporttype = "vwUnkwon"
    End Select
End Function


Public Function nodekind_enum_string(thisNodeKind As NODEKINDs)
    Select Case thisNodeKind
        Case NODEKIND_CONTENT
            nodekind_enum_string = "content"
        Case NODEKIND_EVENT
            nodekind_enum_string = "event"
        Case NODEKIND_FUNCTION
            nodekind_enum_string = "method"
        Case NODEKIND_PARAMETER
            nodekind_enum_string = "param"
        Case NODEKIND_property
            nodekind_enum_string = "property"
        Case NODEKIND_UNKNOWN
            nodekind_enum_string = "unknown"
        Case tli.INVOKE_PROPERTYGET
            nodekind_enum_string = "property"
        Case tli.INVOKE_PROPERTYPUT
            nodekind_enum_string = "property"
        Case tli.INVOKE_FUNC
            nodekind_enum_string = "method"
            
     End Select
End Function

Public Function sitekind_enum_string(theSiteKind As SiteKinds)
    Select Case theSiteKind
        Case SITEKIND_ACTX
            sitekind_enum_string = "actx"
        Case SITEKIND_instance
            sitekind_enum_string = "morphism"
        Case SITEKIND_VWSERVER
            sitekind_enum_string = "vworlds"
        Case Else
            sitekind_enum_string = "actx"
    End Select
End Function

Public Function modelkind_string(theModelkind As MODELKINDs)
    Select Case theModelkind
        Case MODELKINDs.MODELKIND_object
            modelkind_string = "object"
        Case MODELKINDs.MODELKIND_ARRAY
            modelkind_string = "array"
        Case MODELKINDs.MODELKIND_COLLECTION
            modelkind_string = "collection"
        Case MODELKINDs.MODELKIND_VALUE
            modelkind_string = "value"
        Case MODELKINDs.MODELKIND_FUNCDESC
            modelkind_string = "funcdesc"
        Case Else
            modelkind_string = "unknown"
    End Select
End Function

''Enumerations


Public Function vartype_enum_string(ByVal this As VariantTypeConstants, Optional Cascade)
vartype_enum_string = "VT_VARIANT"
 Select Case this
    Case VT_EMPTY:
        vartype_enum_string = "VT_EMPTY"
    Case VT_NULL:
        vartype_enum_string = "VT_NULL"
    Case VT_I2:
        vartype_enum_string = "VT_I2"
    Case VT_I4:
        vartype_enum_string = "VT_I4"
    Case VT_I8:
        vartype_enum_string = "VT_I8"
    Case VT_UI2:
        vartype_enum_string = "VT_UI2"
    Case VT_UI4:
        vartype_enum_string = "VT_UI4"
    Case VT_UI8:
        vartype_enum_string = "VT_UI8"
    Case VT_R4:
        vartype_enum_string = "VT_R4"
    Case VT_R8:
        vartype_enum_string = "VT_R8"
    Case VT_CY:
        vartype_enum_string = "VT_CY"
    Case VT_DATE:
        vartype_enum_string = "VT_DATE"
    Case VT_BSTR:
        vartype_enum_string = "VT_textBSTR"
    Case VT_ERROR:
        vartype_enum_string = "VT_ERROR"
    Case VT_BOOL:
        vartype_enum_string = "VT_BOOL"
    Case VT_VARIANT:
        vartype_enum_string = "VT_VARIANT"
    Case VT_DECIMAL:
        vartype_enum_string = "VT_DECIMAL"
    Case VT_I1:
        vartype_enum_string = "VT_I1"
    Case VT_UI1:
        vartype_enum_string = "VT_UI1"
    Case VT_INT:
        vartype_enum_string = "VT_INT"
    Case VT_UINT:
        vartype_enum_string = "VT_UINT"
    Case VT_VOID:
        vartype_enum_string = "VT_VOID"
    Case VT_SAFEARRAY:
        vartype_enum_string = "VT_SAFEARRAY"
    Case VT_USERDEFINED:
        vartype_enum_string = "VT_USERDEFINED"
    Case VT_LPSTR:
        vartype_enum_string = "VT_LPSTR"
    Case VT_LPWSTR:
        vartype_enum_string = "VT_LPWSTR"
    Case VT_RECORD:
        vartype_enum_string = "VT_RECORD"
    Case VT_FILETIME:
        vartype_enum_string = "VT_FILETIME"
    Case VT_BLOB:
        vartype_enum_string = "VT_BLOB"
    Case VT_STREAM:
        vartype_enum_string = "VT_STREAM"
    Case VT_STORAGE:
        vartype_enum_string = "VT_STORAGE"
    Case VT_STREAMED_OBJECT:
        vartype_enum_string = "VT_STREAMED_object"
    Case VT_STORED_OBJECT:
        vartype_enum_string = "VT_BLOB_object"
    Case VT_CF:
        vartype_enum_string = "VT_CF"
    Case VT_CLSID:
        vartype_enum_string = "VT_CLSID"
 End Select
End Function

Public Function invokekinds_enum_string(ByVal this As tli.InvokeKinds, Optional Cascade = 2)
  Select Case this
    Case INVOKE_EVENTFUNC
      invokekinds_enum_string = "INVOKE_EVENTFUNC" '"Event"
    Case INVOKE_FUNC
      invokekinds_enum_string = "INVOKE_FUNC" '"Method"
    Case INVOKE_PROPERTYGET
      invokekinds_enum_string = "INVOKE_PROPERTYGET" '"Property Get"
    Case INVOKE_PROPERTYPUT
      invokekinds_enum_string = "INVOKE_PROPERTYPUT" '"Property Let"
    Case INVOKE_PROPERTYPUTREF
      invokekinds_enum_string = "INVOKE_PROPERTYPUTREF" '"Property Set"
    Case INVOKE_PROPERTYPUT Or INVOKE_PROPERTYGET
      invokekinds_enum_string = "INVOKE_PROPERTYPUT_PROPERTYGET" '"Property Get/Let"
    Case INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYGET
      invokekinds_enum_string = "INVOKE_PROPERTYPUTREF_PROPERTYGET" '"Property Get/Set"
    Case INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYPUT
      invokekinds_enum_string = "INVOKE_PROPERTYPUTREF_PROPERTYPUT" '"Property Let/Set"
    Case INVOKE_PROPERTYGET Or INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYPUT
      invokekinds_enum_string = "INVOKE_PROPERTYGET_PROPERTYPUTREF_PROPERTYPUT" '"Property Get/Let/Set"
  End Select
  If this And INVOKE_CONST Then invokekinds_enum_string = invokekinds_enum_string & "_INVOKE_CONST"
End Function

Public Function invokekinds_enum_string_io(ByVal this As tli.InvokeKinds, Optional Cascade = 2)
  Select Case this
    Case INVOKE_EVENTFUNC
      invokekinds_enum_string_io = "-" '"Event"
    Case INVOKE_FUNC
     invokekinds_enum_string_io = "-" '"Method"
    Case INVOKE_PROPERTYGET
      invokekinds_enum_string_io = "-" '"Property Get"
    Case INVOKE_PROPERTYPUT
      invokekinds_enum_string_io = "+" '"Property Let"
    Case INVOKE_PROPERTYPUTREF
      invokekinds_enum_string_io = "?" '"Property Set"
    Case INVOKE_PROPERTYPUT Or INVOKE_PROPERTYGET
      invokekinds_enum_string_io = "?" '"Property Get/Let"
    Case INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYGET
      invokekinds_enum_string_io = "?" '"Property Get/Set"
    Case INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYPUT
      invokekinds_enum_string_io = "?" '"Property Let/Set"
    Case INVOKE_PROPERTYGET Or INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYPUT
      invokekinds_enum_string_io = "?" '"Property Get/Let/Set"
  End Select
  'If This And INVOKE_CONST Then invokekinds_enum_string_io = invokekinds_enum_string_io & "_INVOKE_CONST"
End Function

Public Function typekinds_enum_string(ByVal this As TypeKinds, Optional Cascade = 2)
  Select Case this
    Case TKIND_ENUM
        typekinds_enum_string = "TKIND_ENUM"
    Case TKIND_RECORD
        typekinds_enum_string = "TKIND_RECORD"
    Case TKIND_MODULE
        typekinds_enum_string = "TKIND_MODULE"
    Case TKIND_INTERFACE
        typekinds_enum_string = "TKIND_INTERFACE"
    Case TKIND_DISPATCH
        typekinds_enum_string = "TKIND_DISPATCH"
    Case TKIND_ALIAS
        typekinds_enum_string = "TKIND_ALIAS"
    Case TKIND_UNION
        typekinds_enum_string = "TKIND_UNION"
    Case TKIND_MAX
        typekinds_enum_string = "TKIND_MAX"
 End Select
End Function


