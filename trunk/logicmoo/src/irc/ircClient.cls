VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "ircClient"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit
'local variable(s) to hold property value(s)
Private mvarnext_event As Variant 'local copy
Event KeyPress(KeyAscii As Integer)

Public Sub say(channel, message)
    Stop
    Client.SendData "PRIVMSG " & channel & " :" & message
End Sub
Public Function sayres(ByVal channel, ByVal message)
   ' Stop
    Client.SendData "PRIVMSG " & channel & " :" & message
    sayres = "true"
End Function


Public Property Let next_event(ByVal vData As Variant)
'used when assigning a value to the property, on the left side of an assignment.
'Syntax: X.next_event = 5
    mvarnext_event = vData
End Property


Public Property Set next_event(ByVal vData As Variant)
'used when assigning an Object to the property, on the left side of a Set statement.
'Syntax: Set x.next_event = Form1
    Set mvarnext_event = vData
End Property


Public Property Get next_event() As Variant
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.next_event
    If IsObject(mvarnext_event) Then
        Set next_event = mvarnext_event
    Else
        next_event = mvarnext_event
    End If
    next_event = "nil."
    next_event = Split(LTrim(EventBuffer) & vbCrLf, vbCrLf)(0)
    EventBuffer = Split(LTrim(EventBuffer) & vbCrLf, vbCrLf)(1)
    
End Property



Private Sub Class_Initialize()
'Stop
EventBuffer = ""
End Sub
