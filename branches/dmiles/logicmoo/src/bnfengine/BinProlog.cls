VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "BinProlog"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
                                                                                                                                                                                                                      Private PrologObject As BPrologLib.Engine
Private tempStringIn As String
Private tempStringOut As String

Private mvarNetworkProlog As Boolean 'local copy
Public Sub Reset()
Set PrologObject = Nothing
Set PrologObject = New BPrologLib.Engine
End Sub

Public Function Execute(ByVal Query As String) As String
tempStringIn = Query
tempStringOut = PrologObject.CmdProcessor(tempStringIn)
Execute = tempStringOut
End Function

Public Function FindOnce(ByVal VarName As String, ByVal Query As String) As String
tempStringIn = "1:(" & VarName & ")*(" & Query & ")"
tempStringOut = PrologObject.CmdProcessor(tempStringIn)
FindOnce = tempStringOut
End Function

Public Function Consult(ByVal FileName As String) As Boolean
tempStringIn = " ['" & FileName & "']"
tempStringOut = PrologObject.CmdProcessor(tempStringIn)
Consult = tempStringOut
End Function

Public Property Let NetworkProlog(ByVal vData As Boolean)
    mvarNetworkProlog = vData
End Property

Public Property Get NetworkProlog() As Boolean
    NetworkProlog = mvarNetworkProlog
End Property

Private Sub Class_Initialize()
Set PrologObject = New BPrologLib.Engine
End Sub


