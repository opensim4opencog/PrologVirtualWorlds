VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "IDispatchCallback"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public Function GetDISPID(ByVal name As String) As Long
End Function

Public Function InvokeMethod(ByVal InvokeType As InvokeKind, ByVal DISPID As Long, Params() As Variant, Result As Variant) As Long
End Function

Public Property Get InvokeAddr() As Long
End Property

Public Property Let InvokeAddr(ByVal OldAddr As Long)
End Property

Public Property Get GIONAddr() As Long
End Property
Public Property Let GIONAddr(ByVal OldAddr As Long)
End Property

