Attribute VB_Name = "Functions"
Option Explicit

Sub Main()
    installCheck
    preptheobjects
    show_ide

End Sub


Public Function assign(Value, Optional thevalue As Variant) As Variant
    If Not IsMissing(thevalue) Then
        If IsObject(thevalue) Then
            Set Value = thevalue
        Else
            Value = thevalue
        End If
    Else
        assign assign, Value
    End If
End Function


Sub show_ide()

        theMainForm.Show
        thePrologTracer.Show
        theComClassBrowser.Show
        
        theComClassBrowser.Visible = True
        thePrologTracer.Visible = True
        theMainForm.Visible = True
       ' theComBrowser.Visible = True
        
        App.TaskVisible = True
        
End Sub

Sub hide_ide()
        
        App.TaskVisible = False
        
        theComClassBrowser.Visible = False
        theComClassBrowser.Visible = False
        theMainForm.Visible = False
'        theComBrowser.Visible = False
        
End Sub

'End Sub

Public Function GetNextClassDebugID() As Long
    'class ID generator
    Static lClassDebugID As Long
    lClassDebugID = lClassDebugID + 1
    GetNextClassDebugID = lClassDebugID
End Function

Function ReverseChars(ByVal Source As String) As String
Dim i As Long, temp As String
For i = 1 To Len(Source)
temp = Mid(Source, i, 1) & temp
Next i
ReverseChars = temp
End Function

Public Sub Void(foo)

End Sub


Function GetHead(ByVal Source As String) As String
Dim i As Long
i = InStr(Source, vbLf)
If i < 1 Then GetHead = Source Else GetHead = Left(Source, i - 1)
End Function

Function GetTail(ByVal Source As String) As String
Dim i As Long
i = InStr(Source, vbLf)
If i < 1 Then GetTail = "" Else GetTail = Mid(Source, i + 1)
End Function

Public Function countchars(Source, char) As Long
Dim i As Long, output As Long
output = 0
For i = 1 To Len(Source)
If Mid(Source, i, 1) = char Then output = output + 1
Next i
countchars = output
End Function


