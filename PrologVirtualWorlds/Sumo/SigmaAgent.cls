VERSION 1.0 CLASS
BEGIN
  MultiUse = 0   'False
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "SigmaAgent"
Attribute VB_GlobalNameSpace = True
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
'local variable(s) to hold property value(s)
Private mvarUser As Variant 'local copy
Private mvarSocket As Object 'local copy
Private mvarXMLDOC As Variant 'local copy
Public Property Let XMLDOC(ByVal vData As Variant)
'used when assigning a value to the property, on the left side of an assignment.
'Syntax: X.XMLDOC = 5
    mvarXMLDOC = vData
End Property


Public Property Set XMLDOC(ByVal vData As Variant)
'used when assigning an Object to the property, on the left side of a Set statement.
'Syntax: Set x.XMLDOC = Form1
    Set mvarXMLDOC = vData
End Property


Public Property Get XMLDOC() As Variant
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.XMLDOC
    If IsObject(mvarXMLDOC) Then
        Set XMLDOC = mvarXMLDOC
    Else
        XMLDOC = mvarXMLDOC
    End If
End Property



Public Property Set Socket(ByVal vData As Object)
'used when assigning an Object to the property, on the left side of a Set statement.
'Syntax: Set x.Socket = Form1
    Set mvarSocket = vData
End Property


Public Property Get Socket() As Object
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.Socket
    Set Socket = mvarSocket
End Property



Public Function Ask(KIF As Variant) As Variant
End Function

Public Sub Tell(KIF As Variant)
End Sub

Public Property Let User(ByVal vData As Variant)
'used when assigning a value to the property, on the left side of an assignment.
'Syntax: X.User = 5
    mvarUser = vData
End Property


Public Property Set User(ByVal vData As Variant)
'used when assigning an Object to the property, on the left side of a Set statement.
'Syntax: Set x.User = Form1
    Set mvarUser = vData
End Property


Public Property Get User() As Variant
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.User
    If IsObject(mvarUser) Then
        Set User = mvarUser
    Else
        User = mvarUser
    End If
End Property



Public Sub ConsultPrologFile(LocalFileName As Variant)
End Sub

Public Sub CallProlog(ByVal Command As Variant)
Attribute CallProlog.VB_UserMemId = 0
End Sub





