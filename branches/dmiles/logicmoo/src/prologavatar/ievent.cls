VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "ievent"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
'local variable(s) to hold property value(s)
Private mvarverb As String 'local copy
Private mvarvarglist As Variant 'local copy
Private mvareventlist As Collection 'local copy
Private mvariterations As Long 'local copy
Public Sub Invoke(Optional ByVal theAvatar As Object)
Attribute Invoke.VB_UserMemId = 0
If typename(theAvatar) = "Nothing" Then Set theAvatar = theIAvatar
theIAvatar.InvokeMethod mvarverb, Caller.pl_obj(mvarvarglist)

End Sub

Public Property Let iterations(ByVal vData As Long)
'used when assigning a value to the property, on the left side of an assignment.
'Syntax: X.iterations = 5
    mvariterations = vData
End Property


Public Property Get iterations() As Long
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.iterations
    iterations = mvariterations
End Property

Public Property Set eventlist(ByVal vData As Collection)
'used when assigning an Object to the property, on the left side of a Set statement.
'Syntax: Set x.eventlist = Form1
    Set mvareventlist = vData
End Property


Public Property Get eventlist() As Collection
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.eventlist
    Set eventlist = mvareventlist
End Property



Public Property Let varglist(ByVal vData As Variant)
'used when assigning a value to the property, on the left side of an assignment.
'Syntax: X.varglist = 5
    mvarvarglist = vData
End Property


Public Property Set varglist(ByVal vData As Variant)
'used when assigning an Object to the property, on the left side of a Set statement.
'Syntax: Set x.varglist = Form1
    Set mvarvarglist = vData
End Property


Public Property Get varglist() As Variant
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.varglist
    If IsObject(mvarvarglist) Then
        Set varglist = mvarvarglist
    Else
        varglist = mvarvarglist
    End If
End Property



Public Property Let verb(ByVal vData As String)
'used when assigning a value to the property, on the left side of an assignment.
'Syntax: X.verb = 5
    mvarverb = vData
End Property


Public Property Get verb() As String
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.verb
    verb = mvarverb
End Property



