VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 1  'Persistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "a_Existor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"

Option Explicit


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

Private mp As pl_collection


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

Public Function assign(Optional thevalue As Variant) As Variant
    If Not IsMissing(thevalue) Then
        If IsObject(thevalue) Then
            Set Me.Value = thevalue
        Else
            Me.Value = thevalue
        End If
    End If
    theCOMMorphism.assign assign, Me.Value
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


