VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 1  'Persistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "a_Datum"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public Enum NODEKINDs
    NODEKIND_UNKNOWN = tli.InvokeKinds.INVOKE_UNKNOWN
    NODEKIND_EVENT = tli.InvokeKinds.INVOKE_EVENTFUNC
    NODEKIND_FUNCTION = tli.InvokeKinds.INVOKE_FUNC
    NODEKIND_property = (tli.InvokeKinds.INVOKE_PROPERTYPUT Or tli.InvokeKinds.INVOKE_PROPERTYGET)
    NODEKIND_CONTENT = tli.InvokeKinds.INVOKE_PROPERTYPUTREF
    NODEKIND_PARAMETER = tli.INVOKE_CONST
End Enum


Public Enum MODELKINDs
    MODELKIND_FUNCDESC = 256 * 1
    MODELKIND_object = 256 * 2
    MODELKIND_COLLECTION = 265 * 4
    MODELKIND_VALUE = 256 * 8
    MODELKIND_ARRAY = 256 * 16
   ' SITEKIND_MAP = 256 * 32
   ' SITEKIND_LIST = 256 * 64
End Enum
    
Public Type CollectionMember
    TheItem As Variant
    TheKey As Variant
End Type

Public Enum MorphismKinds
    MorphismKIND_INTERFACE
    MorphismKIND_INDEPENDENT
End Enum

Public Enum SiteKinds
    SITEKIND_UNKNOWN = 0
    SITEKIND_UNBOUND = 1
    SITEKIND_REFDATA = 32
    SITEKIND_VWTHING = 64
    SITEKIND_VWSERVER = 128
    SITEKIND_ACTX = 256
    SITEKIND_instance = 512
    SITEKIND_PROLOG_SERVER = 1024
    SITEKIND_PROLOG_CLIENT = 2048
End Enum

Public Enum InvokeKinds
  INVOKE_UNKNOWN = 0
  INVOKE_FUNC = 1
  INVOKE_PROPERTYGET = 2
  INVOKE_PROPERTYPUT = 4
  INVOKE_PROPERTYPUTREF = 8
  'Special TLI values
  INVOKE_EVENTFUNC = 16
  INVOKE_CONST = 32
End Enum

Public Enum TypeKind
 TKIND_ENUM = 0 'enum Constants Set of related Long constant values
 TKIND_RECORD = 1 'record Records Ordered data members of intrinsic or defined types
 TKIND_MODULE = 2 'module Declarations Constants List of Dll-declared Public Functions and/or constant values of any type.
 TKIND_INTERFACE = 3 'interface Interfaces Description of an IUnknown derived vtable
 TKIND_DISPATCH = 4 'dispinterface Interfaces Description of an IDispatch derived object. Members are accessible using IDispatch::Invoke only, unless the dispinterface is also dual.
 TKIND_COCLASS = 5 'coclass CoClasses An object which can be created using Cocreateinstance (CreateObject or New in VB). A coclass is a list of interfaces.
 TKIND_ALIAS = 6 'alias IntrinsicAliases <All> An alias to an intrinsic type, or any of the other TypeKinds.
 TKIND_UNION = 7 'union Unions A description of one or more records which can share the same block of memory.
 TKIND_MAX = 8 ' N/A N/A Returned by TypeKind if data not available
End Enum


'Implements IDispatchCallback

Public MemberName As String
Public nodekind As NODEKINDs
Public MODELKIND As MODELKINDs
Public SiteKind As SiteKinds
Public Origin As Object
'Public TypeName As String

Public NativeTLI As TypeLibInfo
Public Models As PropertyMap

Public Dirty As Boolean
Public Transaction As Long
Public m_Value As Variant
Public m_TypeName As String

Private InvokeAddress As Long
Private GIONAddress As Long

Property Set Value(vData)
    'Stop
        SiteKind = theCOMMorphism.sitekind_from_object(vData)
        Select Case SiteKind
            Case MODELKIND_COLLECTION, MODELKINDs.MODELKIND_ARRAY
                Set m_Value = vData
            Case Else
                If nodekind <> NODEKIND_property Then
                    Set m_Value = vData
                    Exit Property
                End If
                
                If Origin Is Nothing Or MemberName = "" Then
                    Set m_Value = vData
                    
                Else
                    If IsObject(vData) Then
                        theCOMMorphism.call_by_Name_pred m_Value, Origin, MemberName, VbSet, vData
                    Else
                        theCOMMorphism.call_by_Name_pred m_Value, Origin, MemberName, VbLet, vData
                    End If
                End If
        End Select
End Property

Public Property Get typeName()
    If m_TypeName = "" Then typeName = VBA.typeName(m_Value) Else typeName = m_TypeName
End Property

Public Property Let typeName(vType)
    m_TypeName = vType
End Property


Public Property Let Value(ByVal vData As Variant)
'Stop
    m_Value = vData
End Property
Public Property Get Value() As Variant
   On Error Resume Next
    If Origin Is Nothing Or MemberName = "" Or InStr(MemberName, "objptr") Then
        If IsObject(m_Value) Then
            Set Value = m_Value
        Else
            Value = m_Value
        End If
    Else
        theCOMMorphism.call_by_Name_pred m_Value, Origin, MemberName, VbGet
'        Stop
        If IsObject(m_Value) Then
            Set Value = m_Value
        Else
            Value = m_Value
        End If
    End If
'Stop
End Function



Public Function prolog(Optional ByVal Level = 2) As String
prolog = theCOMMorphism.datum_to_prolog(Me, Level)
End Function


Private Sub Class_Initialize()
'HookDispatch Me
End Sub

Private Sub Class_Terminate()
'UnHookDispatch Me
End Sub

'Private Function IDispatchCallback_GetDISPID(ByVal Name As String) As Long
'    If Not Models.IsValid(Name) Then
'        IDispatchCallback_GetDISPID = DISPID_UNKNOWN
'        Exit Function
'    End If
'    IDispatchCallback_GetDISPID = ObjPtr(Models.Property(Name))
'End Function

Private Property Let IDispatchCallback_GIONAddr(ByVal RHS As Long)
   GIONAddress = RHS
End Property


Private Property Get IDispatchCallback_GIONAddr() As Long
   IDispatchCallback_GIONAddr = GIONAddress
End Property

Private Property Let IDispatchCallback_InvokeAddr(ByVal RHS As Long)
   InvokeAddress = RHS
End Property

Private Property Get IDispatchCallback_InvokeAddr() As Long
   IDispatchCallback_InvokeAddr = InvokeAddress
End Property


'Private Function IDispatchCallback_InvokeMethod(ByVal InvokeType As tli.InvokeKinds, ByVal DISPID As Long, Params() As Variant, Result As Variant) As Long
'Dim Name As String
'      Name = theKnownMorphs.Property(CStr(DISPID)).MemberName
'      theCOMMorphism.call_by_Name_pred Result, m_Value, Name, InvokeType, Params
' Exit Function
'   On Error Resume Next
'    IDispatchCallback_InvokeMethod = DISP_E_TYPEMISMATCH
'    IDispatchCallback_InvokeMethod = DISP_E_BADPARAMCOUNT
'    IDispatchCallback_InvokeMethod = DISP_E_UNKNOWNName
'End Function


Public Function to_activex()
    Set to_activex = m_Value
End Function


