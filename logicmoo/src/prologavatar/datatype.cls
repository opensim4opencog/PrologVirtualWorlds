VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "datum"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Private This As Variant
Private thisOrigin As Object
Private thisMemberName As String
Private thisDataKind As DataKinds
Private thisContents As PropertyMap


Public Property Get Value() As Variant
    If thisOrigin Is Nothing Then
        theMorphism.assign Value, This
    Else
        theMorphism.assign Value, CallByName(thisOrigin, thisMemberName, VbGet)
    End If
End Function

Public Function assign(Optional thevalue As Variant) As Variant
    If thisOrigin Is Nothing Then
        theMorphism.assign This, thevalue
    Else
        If IsObject(thevalue) Then
            theMorphism.call_by_name_pred assign, thisOrigin, thisMemberName, VbSet, thevalue
        Else
            theMorphism.call_by_name_pred assign, thisOrigin, thisMemberName, VbLet, thevalue
        End If
    End If
    
    theMorphism.assign assign, Me.Value
End Function


Public Property Let Value(ByVal vData As Variant)
    Void Me.assign(vData)
End Property

Public Property Set Value(ByVal vData As Variant)
    Void Me.assign(vData)
End Property

Public Sub assign_prolog(ByVal sObject As String)
    theMorphism.pl_obj_pred This, sObject
End Sub
Public Sub SetOrigin(sObject, memberName)
    Set thisOrigin = sObject
    thisMemberName = memberName
End Sub

Private Sub Class_Initialize()
    theAddRefs.Add Me, CStr(ObjPtr(Me))
End Sub

Private Sub Class_Terminate()
    'theAddRefs.Remove CStr(ObjPtr(Me))
End Sub
