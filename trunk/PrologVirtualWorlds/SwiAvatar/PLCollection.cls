VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 1  'Persistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "pl_collection"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Collection" ,"Datum"
Attribute VB_Ext_KEY = "Member0" ,"Datum"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Private mCol As Variant
Private mColArray As Collection
Private prologIndexElement As Long
Private totalElements As Long

'Implements PrologEvents

Public Property Let Value(ByVal vData As Variant)
    Me.Add vData
End Property


Public Property Set Value(ByVal vData As Variant)
    Me.Add vData
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
        theMorphism.pl_object_pred mCol, Arg1
        Me.Enumerate
    Else
        theMorphism.pl_object_pred Arg1, Arg2
        theMorphism.pl_object_pred mCol, Arg2
        Me.Enumerate
    End If
End Sub

Public Property Get Count() As Long
    'used when retrieving the number of elements in the
    'collection. Syntax: Debug.Print x.Count
    Count = mColArray.Count
End Property


Public Sub MoveNext()
    Me.pl_redo
End Sub

Public Sub MoveFirst()
    Me.pl_entry
End Sub

Public Sub MoveLast()
    prologIndexElement = totalElements
End Sub

Public Sub pl_exception()
    Me.pl_redo
End Sub

Public Sub pl_fail()
    Me.pl_nondeterm_fail
End Sub

Public Sub pl_nondeterm_fail()
    Me.pl_redo
End Sub

Public Sub pl_deterministic_fail()
    prologIndexElement = 0
End Sub

Public Sub pl_redo()
    prologIndexElement = prologIndexElement + 1
End Sub

Public Sub pl_cut()
    'prologIndexElement = 0
End Sub

Public Sub pl_exit()
   Me.pl_nondeterm_exit
End Sub

Public Sub pl_nondeterm_exit()
    'prologIndexElement = 0
End Sub

Public Sub pl_deterministic_exit()
    'prologIndexElement=0
End Sub

Public Sub pl_call()
    prologIndexElement = 0
End Sub

Public Sub pl_entry()
    prologIndexElement = 0
End Sub

Public Sub pl_list(List_BSTR)
    Me.assign List_BSTR
End Sub


Public Sub Enumerate()
 Dim subElement
    totalElements = -1
    For Each subElement In mCol
        mColArray.Add subElement, totalElements
        totalElements = totalElements + 1
    Next
End Sub


Public Function Add(Item, Optional Key, Optional Before, Optional After)
    mCol.Add Item, Key, Before, After
    totalElements = totalElements + 1
    mColArray.Add Item, totalElements
    If IsObject(Key) Then Set Add = Key
End Function

Public Property Get Item(Key As Variant)
    'used when referencing an element in the collection
    'Key contains either the Index or Key to the collection,
    'this is why it is declared as a Variant
    'Syntax: Set foo = x.Item(xyz) or Set foo = x.Item(5)
  Set Item = mCol(Key)
End Property

Public Sub Remove(Key As Variant)
    'used when removing an element from the collection
    'Key contains either the Index or Key, which is why
    'it is declared as a Variant
    'Syntax: x.Remove(xyz)
    
    mCol.Remove Key
    Me.Enumerate
End Sub

Public Property Get NewEnum() As IUnknown
    ' this property allows you to enumerate
    ' this collection with the For...Each syntax
    Set NewEnum = mCol.[_NewEnum]
End Property

Private Sub Class_Initialize()
    'creates the collection when this class is created
    Set mCol = New Collection
    totalElements = -1
    prologIndexElement = 0
End Sub

Private Sub Class_Terminate()
    'destroys collection when this class is terminated
    Set mCol = Nothing
End Sub

