VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 1  'Persistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "PrologEvents"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Private mColArray As Collection
Private prologIndexElement As Long
Private totalElements As Long

'Implements PrologEvents

Public Property Let Value(ByVal vData As Variant)
    'Me.Add vData
End Property


Public Property Set Value(ByVal vData As Variant)
    'Me.Add vData
End Property


Public Property Get Value() As Variant
    If prologIndexElement > totalElements Then
        Value = Null
    Else
        Value = mColArray.Item(prologIndexElement)
    End If
End Property

Public Sub assign(Arg1, Optional Arg2)
    prologIndexElement = 0
    If IsMissing(Arg2) Then
        'theMorphism.pl_object_pred mCol, Arg1
        'Me.Enumerate
    Else
        'theMorphism.pl_object_pred Arg1, Arg2
        'theMorphism.pl_object_pred mCol, Arg2
        'Me.Enumerate
    End If
End Sub

Public Property Get Count() As Long
    'used when retrieving the number of elements in the
    'collection. Syntax: Debug.Print x.Count
    Count = mColArray.Count
End Property


Public Sub MoveNext()
    prologIndexElement = prologIndexElement + 1
End Sub

Public Sub MoveFirst()
    prologIndexElement = 0
End Sub

Public Sub MoveLast()
    prologIndexElement = totalElements
End Sub

Public Sub pl_exception()
    prologIndexElement = prologIndexElement + 1
End Sub

Public Sub pl_fail()
    prologIndexElement = prologIndexElement + 1
End Sub

Public Sub pl_redo()
    prologIndexElement = prologIndexElement + 1
End Sub

Public Sub pl_cut()
    'prologIndexElement = 0
End Sub

Public Sub pl_nondeterm_exit()
    'prologIndexElement = 0
End Sub

Public Sub pl_deterministic_exit()
    'prologIndexElement=0
End Sub

Public Sub pl_exit()
    'prologIndexElement=0
End Sub

Public Sub pl_call()
    prologIndexElement = 0
End Sub

Public Sub pl_list()
    prologIndexElement = 0
End Sub



