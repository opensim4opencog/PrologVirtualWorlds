VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "InstanceHolder"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
'local variable(s) to hold property value(s)
Private mvarTheInstance As Variant 'local copy
Private mvartheReference As Variant 'local copy
Public Property Let theReference(ByVal vData As Variant)
'used when assigning a value to the property, on the left side of an assignment.
'Syntax: X.theReference = 5
    mvartheReference = vData
End Property


Public Property Set theReference(ByVal vData As Variant)
'used when assigning an Object to the property, on the left side of a Set statement.
'Syntax: Set x.theReference = Form1
    Set mvartheReference = vData
End Property


Public Property Get theReference() As Variant
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.theReference
    If IsObject(mvartheReference) Then
        Set theReference = mvartheReference
    Else
        theReference = mvartheReference
    End If
End Property



Public Property Let TheInstance(ByVal vData As Variant)
'used when assigning a value to the property, on the left side of an assignment.
'Syntax: X.TheInstance = 5
    mvarTheInstance = vData
End Property


Public Property Set TheInstance(ByVal vData As Variant)
'used when assigning an Object to the property, on the left side of a Set statement.
'Syntax: Set x.TheInstance = Form1
    Set mvarTheInstance = vData
End Property


Public Property Get TheInstance() As Variant
Attribute TheInstance.VB_UserMemId = 0
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.TheInstance
    If IsObject(mvarTheInstance) Then
        Set TheInstance = mvarTheInstance
    Else
        TheInstance = mvarTheInstance
    End If
End Property



