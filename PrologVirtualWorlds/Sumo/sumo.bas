Attribute VB_Name = "Main_Module"

Public fAskTell As AskTell
Public ResponseForm As frmDocument
Public fTelnetSpy As frmDocument
Public DefaultContext As String
Public OptionSwitches As String
Public CallingXML As Boolean
'Public ClosingApp
Public XMLBuffer As String
Public XMLD As DOMDocument
Public AllOptions As String
Public OptionString(200) As String



Sub Main()
    AllOptions = ""
    
    AllOptions = AllOptions & "Interpreted (CycL/KIF):[opt_kb=prolog,opt_precompiled=false,opt_language=cycl,opt_notation=lisp,disp_cputime]" & vbCrLf
    AllOptions = AllOptions & "Compiled (CycL/KIF):[opt_kb=prolog,opt_precompiled,opt_language=hylog,opt_notation=lisp,disp_cputime]" & vbCrLf
    AllOptions = AllOptions & "Compiled (Prolog):[opt_kb=prolog,opt_precompiled,opt_language=hylog,opt_notation=prolog,disp_cputime]" & vbCrLf
    AllOptions = AllOptions & "Prolog:[]" & vbCrLf
    AllOptions = AllOptions & "Prolog Defaults:[]" & vbCrLf
    AllOptions = AllOptions & "Debugging:[disp_notes_nonuser,disp_debug]" & vbCrLf
    
    
    Set XMLD = New DOMDocument
    ClosingApp = False
    Set fMainForm = New frmMain
    fMainForm.Show
    Set fAskTell = New AskTell
    fAskTell.Show
    Set ResponseForm = New frmDocument
    ResponseForm.Caption = "Results"
    ResponseForm.Show
    Set fTelnetSpy = New frmDocument
    fTelnetSpy.Hide
    fTelnetSpy.Caption = "Telnet Log"

    CallProlog "ssi."

End Sub
 Sub shutdownApp()
Set XMLD = Nothing
fMainForm.telnet.Close
'Unload fMainForm.telnet

Set fTelnetSpy = Nothing
Set fAskTell = Nothing
Set ResponseForm = Nothing

'    Stop
    fMainForm.ClosingApp.Value = 1
'    Unload fAskTell
    If fMainForm.WindowState <> vbMinimized And fMainForm.WindowState <> vbMaximized Then
        SaveSetting App.Title, "Settings", "MainLeft", fMainForm.Left
        SaveSetting App.Title, "Settings", "MainTop", fMainForm.Top
        SaveSetting App.Title, "Settings", "MainWidth", fMainForm.Width
        SaveSetting App.Title, "Settings", "MainHeight", fMainForm.Height
    End If
    Unload fMainForm
    End
End Sub
Sub CallProlog(theText As String)
    CallingXML = True
    XMLBuffer = ""
    On Error Resume Next
    fMainForm.telnet.Close
    fMainForm.telnet.RemoteHost = fMainForm.Server
    fMainForm.telnet.RemotePort = fMainForm.Port
    fMainForm.telnet.Connect fMainForm.Server, fMainForm.Port
    SendTelnetLogLn "// Connecting... " & fMainForm.Server & ":" & fMainForm.Port
    While (fMainForm.telnet.State <> 7 And fMainForm.telnet.State <> 9): DoEvents: DoEvents: DoEvents: DoEvents: DoEvents: DoEvents: Wend
    SendTelnetLogLn "// Connected"
    fMainForm.telnet.SendData theText & " " & vbCrLf
    SendTelnetLogLn "// Sent: " & Chr(34) & theText & Chr(34)
    'While fMainForm.telnet.State = 7: DoEvents: DoEvents: DoEvents: Wend
End Sub

Sub SendTelnetLog(theText As String)
    On Error Resume Next
    theText = RTrim(theText)
    fTelnetSpy.theWindow.Text = fTelnetSpy.theWindow.Text & theText
    fTelnetSpy.theWindow.SelStart = Len(fTelnetSpy.theWindow.Text)
End Sub

Sub SendTelnetLogLn(theText As String)
    SendTelnetLog theText & vbCrLf
End Sub

Function SafeText(ByRef theText As String)
    SafeText = Trim(theText)
    While (Right(SafeText, 1) = ".")
        SafeText = RTrim(Left(SafeText, Len(SafeText) - 1))
    Wend
End Function

Sub ParseXML()
    On Error Resume Next
    Dim LETop As IXMLDOMElement
    Dim ChildE As IXMLDOMElement
    
    ResponseForm.theWindow.Text = ""
    XMLD.loadXML XMLBuffer
'    Stop
    Set LETop = XMLD.getElementsByTagName("LE")(0)
    If LETop Is Nothing Then Set LETop = XMLD.getElementsByTagName("Answer")(0)
   ' Stop
    If LETop Is Nothing Then
            ResponseForm.theWindow.Text = ResponseForm.theWindow.Text & Replace(Replace(XMLBuffer, vbCrLf & vbCrLf, vbCrLf), vbCrLf & vbCrLf, vbCrLf) & vbCrLf

    End If
    For Each ChildE In LETop.childNodes
        UserPostXML ChildE
    Next
    For Each yes In XMLD.getElementsByTagName("YES")
        TF = 1
    Next
    If TF = 1 Then
        ResponseForm.theWindow.Text = ResponseForm.theWindow.Text & "YES." & vbCrLf
    Else
        ResponseForm.theWindow.Text = ResponseForm.theWindow.Text & "NO." & vbCrLf
    End If
   ' MsgBox ResponseForm.theWindow.rtftext
    ResponseForm.Show
    ResponseForm.SetFocus
    ResponseForm.Visible = True

End Sub
Sub UserPostXML(ChildE As IXMLDOMElement)
Dim Var As IXMLDOMElement
Select Case LCase(ChildE.nodeName)
    Case "note"
        ResponseForm.theWindow.Text = ResponseForm.theWindow.Text & ChildE.getAttribute("class") & "=" & ChildE.Text & vbCrLf & vbCrLf
    Case "result"
        For Each Var In ChildE.getElementsByTagName("var")
            ResponseForm.theWindow.Text = ResponseForm.theWindow.Text & Var.getAttribute("name") & "=" & Var.Text & vbCrLf
        Next
        ResponseForm.theWindow.Text = ResponseForm.theWindow.Text & vbCrLf
    Case "consult"
End Select
End Sub



 Sub LoadNewDoc()
    Static lDocumentCount As Long
    Dim frmD As frmDocument
    lDocumentCount = lDocumentCount + 1
    Set frmD = New frmDocument
    frmD.Caption = "Document " & lDocumentCount
    frmD.Show
End Sub

Function Head(ByRef KIF As String)
    KIF = Replace(KIF, vbCr, " ")
    KIF = Replace(KIF, vbLf, " ")
    KIF = Replace(KIF, "  ", " ")
    KIF = Replace(KIF, "  ", " ")
    KIF = Replace(KIF, Chr(34), "\" & Chr(34))
    
    Head = KIF: KIF = "": Exit Function
    
    
    For i = 1 To Len(KIF)
    ch = Mid(KIF, i, 1)
    Select Case ch
        Case "("
            BrackDepth = BrackDepth + 1
        Case ")"
            BrackDepth = BrackDepth - 1
            If BrackDepth = 0 Then
                If Mid(KIF, i + 1, 1) = "." Then i = i + 1
                Head = Left(KIF, i)
                KIF = Mid(KIF, i + 1)
'                Stop
                Exit Function
            End If
    
    End Select
    Next
    Head = (KIF)
 '   Stop
    KIF = ""
End Function
