VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Begin VB.Form Channel 
   Caption         =   "#channel"
   ClientHeight    =   5925
   ClientLeft      =   6870
   ClientTop       =   1305
   ClientWidth     =   8190
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "frmChannel.frx":0000
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   NegotiateMenus  =   0   'False
   ScaleHeight     =   5925
   ScaleWidth      =   8190
   Begin MSComctlLib.Toolbar Toolbar1 
      Height          =   3630
      Left            =   6570
      TabIndex        =   10
      Top             =   60
      Width           =   345
      _ExtentX        =   609
      _ExtentY        =   6403
      ButtonWidth     =   609
      ButtonHeight    =   582
      Style           =   1
      ImageList       =   "ImageList1"
      _Version        =   393216
      BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
         NumButtons      =   11
         BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Object.ToolTipText     =   "Click to Op/DeOp the selected user."
            ImageIndex      =   3
         EndProperty
         BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Object.ToolTipText     =   "Click to give/take away the selected user helper status."
            ImageIndex      =   5
         EndProperty
         BeginProperty Button3 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Object.ToolTipText     =   "Click to Voice/DeVoice the selected user."
            ImageIndex      =   4
         EndProperty
         BeginProperty Button4 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "t"
            ImageIndex      =   6
            Style           =   1
         EndProperty
         BeginProperty Button5 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "i"
            ImageIndex      =   7
            Style           =   1
         EndProperty
         BeginProperty Button6 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "k"
            ImageIndex      =   8
            Style           =   1
         EndProperty
         BeginProperty Button7 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "l"
            ImageIndex      =   9
            Style           =   1
         EndProperty
         BeginProperty Button8 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "m"
            ImageIndex      =   10
            Style           =   1
         EndProperty
         BeginProperty Button9 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "n"
            ImageIndex      =   11
            Style           =   1
         EndProperty
         BeginProperty Button10 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "p"
            ImageIndex      =   12
            Style           =   1
         EndProperty
         BeginProperty Button11 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "s"
            ImageIndex      =   13
            Style           =   1
         EndProperty
      EndProperty
   End
   Begin VB.PictureBox picTopic 
      BorderStyle     =   0  'None
      Height          =   255
      Left            =   1755
      ScaleHeight     =   255
      ScaleWidth      =   4740
      TabIndex        =   8
      Top             =   105
      Width           =   4740
      Begin RichTextLib.RichTextBox rtbTopic 
         Height          =   375
         Left            =   -45
         TabIndex        =   9
         Top             =   -45
         Width           =   4890
         _ExtentX        =   8625
         _ExtentY        =   661
         _Version        =   393217
         Enabled         =   -1  'True
         MultiLine       =   0   'False
         MaxLength       =   512
         TextRTF         =   $"frmChannel.frx":058A
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Lucida Sans Unicode"
            Size            =   9
            Charset         =   255
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
      End
   End
   Begin VB.PictureBox picFlat 
      BackColor       =   &H00800000&
      BorderStyle     =   0  'None
      Height          =   3195
      Left            =   165
      ScaleHeight     =   3195
      ScaleWidth      =   6330
      TabIndex        =   1
      Top             =   390
      Width           =   6330
      Begin VB.PictureBox picNicks 
         BorderStyle     =   0  'None
         Height          =   2925
         Left            =   4680
         ScaleHeight     =   2925
         ScaleWidth      =   1650
         TabIndex        =   6
         Top             =   0
         Width           =   1650
         Begin VB.ListBox lstNicks 
            Height          =   3000
            IntegralHeight  =   0   'False
            Left            =   -30
            MultiSelect     =   2  'Extended
            Sorted          =   -1  'True
            TabIndex        =   7
            Top             =   -45
            Width           =   1710
         End
      End
      Begin VB.PictureBox picDO 
         BorderStyle     =   0  'None
         Height          =   240
         Left            =   0
         ScaleHeight     =   240
         ScaleWidth      =   6360
         TabIndex        =   2
         Top             =   2955
         Width           =   6360
         Begin RichTextLib.RichTextBox DataOut 
            Height          =   390
            Left            =   -45
            TabIndex        =   3
            Top             =   -45
            Width           =   6510
            _ExtentX        =   11483
            _ExtentY        =   688
            _Version        =   393217
            Enabled         =   -1  'True
            MultiLine       =   0   'False
            TextRTF         =   $"frmChannel.frx":0614
            BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
               Name            =   "Lucida Sans Unicode"
               Size            =   9
               Charset         =   255
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
         End
      End
      Begin RichTextLib.RichTextBox DataIn 
         Height          =   2925
         Left            =   0
         TabIndex        =   4
         Top             =   0
         Width           =   4655
         _ExtentX        =   8202
         _ExtentY        =   5159
         _Version        =   393217
         BorderStyle     =   0
         Enabled         =   -1  'True
         HideSelection   =   0   'False
         ReadOnly        =   -1  'True
         ScrollBars      =   2
         Appearance      =   0
         AutoVerbMenu    =   -1  'True
         TextRTF         =   $"frmChannel.frx":069E
         MouseIcon       =   "frmChannel.frx":0728
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Lucida Sans Unicode"
            Size            =   9
            Charset         =   255
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
      End
   End
   Begin MSComctlLib.ImageList ImageList1 
      Left            =   6360
      Top             =   2010
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   13
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmChannel.frx":0A42
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmChannel.frx":0E96
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmChannel.frx":12EA
            Key             =   ""
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmChannel.frx":1606
            Key             =   ""
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmChannel.frx":1922
            Key             =   ""
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmChannel.frx":1C3E
            Key             =   ""
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmChannel.frx":1F5A
            Key             =   ""
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmChannel.frx":2276
            Key             =   ""
         EndProperty
         BeginProperty ListImage9 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmChannel.frx":2592
            Key             =   ""
         EndProperty
         BeginProperty ListImage10 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmChannel.frx":28AE
            Key             =   ""
         EndProperty
         BeginProperty ListImage11 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmChannel.frx":2BCA
            Key             =   ""
         EndProperty
         BeginProperty ListImage12 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmChannel.frx":2EE6
            Key             =   ""
         EndProperty
         BeginProperty ListImage13 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmChannel.frx":3202
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.Toolbar Toolbar 
      Height          =   330
      Left            =   1440
      TabIndex        =   0
      Top             =   3570
      Visible         =   0   'False
      Width           =   705
      _ExtentX        =   1244
      _ExtentY        =   582
      ButtonWidth     =   609
      ButtonHeight    =   582
      Style           =   1
      ImageList       =   "ImageList1"
      _Version        =   393216
      BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
         NumButtons      =   2
         BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            ImageIndex      =   1
         EndProperty
         BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            ImageIndex      =   2
         EndProperty
      EndProperty
   End
   Begin VB.Shape shpTopic 
      BackStyle       =   1  'Opaque
      BorderColor     =   &H00800000&
      BorderWidth     =   2
      Height          =   300
      Left            =   1740
      Top             =   90
      Width           =   4785
   End
   Begin VB.Label lblName 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "#channel name"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   210
      Left            =   120
      TabIndex        =   5
      Top             =   75
      Width           =   1425
   End
   Begin VB.Shape shpDI 
      BorderColor     =   &H00800000&
      BorderWidth     =   2
      Height          =   3240
      Left            =   1650
      Top             =   375
      Width           =   4875
   End
   Begin VB.Shape shpLeftC 
      BackColor       =   &H00800000&
      BackStyle       =   1  'Opaque
      BorderStyle     =   0  'Transparent
      Height          =   3930
      Left            =   0
      Top             =   0
      Width           =   1650
   End
End
Attribute VB_Name = "Channel"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public strTopic As String
Public strMode  As String
Public strName  As String
Public strKey   As String
Public intLimit As Integer
Dim Nicks()     As Nick
Public intNicks As Integer
Public bControl As Boolean
Public bdown As Boolean
Dim Modes()     As typMode
Public intModes As Integer
Public newBuffer As Boolean
Dim bLink As Boolean
Dim strLink As String
Public intUserCount As Integer

Dim textHistory As New Collection
Dim intCurHist  As Integer
Public Sub AddMode(strMode As String, bPlus As Boolean)
    Dim i As Integer
    
    For i = 1 To Toolbar1.Buttons.Count
        If Toolbar1.Buttons.Item(i).Key = strMode Then
            Toolbar1.Buttons.Item(i).Value = tbrPressed
        End If
    Next i
    
    For i = 1 To intModes
        If Modes(i).mode = strMode Then Exit Sub
    Next i
    
    intModes = intModes + 1
    ReDim Preserve Modes(1 To intModes) As typMode
    
    With Modes(intModes)
        .bPos = True
        .mode = strMode
    End With
    Update
End Sub


Public Sub AddNick(strNick As String)
    Dim strPre As String

    If strNick = "" Then Exit Sub
    intNicks = intNicks + 1
    ReDim Preserve Nicks(1 To intNicks) As Nick
    If InStr(strNick, "%") Then Nicks(intNicks).helper = True: strPre = "%": strNick = Replace(strNick, "%", "")
    If InStr(strNick, "+") Then Nicks(intNicks).voice = True: strPre = "+": strNick = Replace(strNick, "+", "")
    If InStr(strNick, "@") Then Nicks(intNicks).op = True: strPre = "@": strNick = Replace(strNick, "@", "")
    
    Nicks(intNicks).Nick = strNick
    lstNicks.AddItem DisplayNick(Nicks(intNicks))
    intUserCount = intUserCount + 1
    Update
End Sub



Public Sub ChangeNck(strOldNick, strNewNick)
    Dim i As Integer, bInd As Integer
    
    For i = 1 To intNicks
        'MsgBox Nicks(i).Nick & "~" & strOldNick
        If LCase(Nicks(i).Nick) = LCase(strOldNick) Then
            Nicks(i).Nick = strNewNick
            bInd = i
            Exit For
        End If
    Next i
    For i = 0 To lstNicks.ListCount - 1
        'MsgBox RealNick(lstNicks.List(i)) & "!" & strOldNick
        If LCase(RealNick(lstNicks.List(i))) = LCase(strOldNick) Then
            lstNicks.List(i) = DisplayNick(Nicks(bInd))
            PutData DataIn, strColor & "03" & strBold & strOldNick & strBold & " is now known as " & strBold & strNewNick
            Exit For
        End If
    Next i
End Sub




Sub DoConnect(strServer As String)
    Dim bDo As Integer
    
    bDo = MsgBox("Would you really like to connect to the server '" & strServer & "'? You will be disconnect from your current one", vbQuestion Or vbYesNo)
    
    If bDo = vbYes Then
        Client.mnu_File_Disconnect_Click
        Dim x As Integer
        GetAlias "", "server " & strServer
    End If
End Sub

Sub DoHistory(strText As String)
    If textHistory.Count > MAX_TEXT_HISTORY Then
        textHistory.Remove 1
    End If
    textHistory.Add strText
    intCurHist = textHistory.Count + 1
End Sub

Public Function GetNick(intIndex As Integer) As String
    GetNick = Nicks(intIndex).Nick
End Function


Function InChannel(strNick As String) As Boolean
    Dim i As Integer
    For i = 1 To intNicks
        If LCase(strNick) = LCase(Nicks(i).Nick) Then InChannel = True: Exit Function
    Next i
    InChannel = False
End Function

Public Function isHalfOp(strNick As String) As Boolean
    Dim i As Integer
    For i = 1 To intNicks
        If LCase(Nicks(i).Nick) = LCase(strNick) Then
            If Nicks(i).helper = True Then
                isHalfOp = True
                Exit Function
            Else
                isHalfOp = False
                Exit Function
            End If
        End If
    Next i
    isHalfOp = False
End Function

Public Function isOp(strNick As String) As Boolean
    Dim i As Integer
    For i = 1 To intNicks
        If LCase(Nicks(i).Nick) = LCase(strNick) Then
            If Nicks(i).op = True Then
                isOp = True
                Exit Function
            Else
                isOp = False
                Exit Function
            End If
        End If
    Next i
    isOp = False
End Function

Public Function isVoice(strNick As String) As Boolean
    Dim i As Integer
    For i = 1 To intNicks
        If LCase(Nicks(i).Nick) = LCase(strNick) Then
            If Nicks(i).voice = True Then
                isVoice = False
                Exit Function
            Else
                isVoice = True
                Exit Function
            End If
        End If
    Next i
    isVoice = False
End Function

Public Function ModeString() As String
    If intModes = 0 Then Exit Function
    Dim strFinal As String, bWhich As Boolean, i As Integer
    If Modes(1).bPos = True Then bWhich = True
    
    If bWhich Then strFinal = strFinal & "+" Else strFinal = strFinal & "-"
    
    For i = 1 To intModes
        If Modes(i).bPos <> bWhich Then
            bWhich = Not bWhich
            If bWhich Then strFinal = strFinal & "+" Else strFinal = strFinal & "-"
        End If
        strFinal = strFinal & Modes(i).mode
    Next i
    ModeString = strFinal
End Function

Public Sub PutText(strNick As String, strText As String)
    'If Left(strText, 8) = strAction & "ACTION " Then
    '    strText = RightR(strText, 8)
    '    strText = LeftR(strText, 1)
    '    PutData Me.DataIn, strColor & "06" & strNick & " " & strText
    'ElseIf Left(strText, 9) = strAction & "VERSION" & strAction Then
    '    'MsgBox "hey"
    '    Client.SendData "CTCPREPLY " & strNick & " VERSION :projectIRC for Windows"
    If Left(strText, 1) = strAction Then
        HandleCTCP strNick, strText
    Else
        'Dim i As Integer
        'For i = 0 To lstNicks.ListCount
        '    If RealNick(lstNicks.List(i)) = strNick Then
        '        If lstNicks.Selected(i) Then
        '            Exit Sub
        '        End If
        '    End If
        'Next i
        PutData Me.DataIn, Trim("" & strNick & " : " & Chr(9) & strText)
    End If
End Sub


Public Sub HandleCTCP(strNick As String, strData As String)
    strData = RightR(strData, 1)
    strData = LeftR(strData, 1)
    
    Dim strCom As String, strParam As String, inttemp As Integer, strTemp As String, strArgs() As String
    Dim dccinfo As DCC_INFO
    
    Seperate strData, " ", strCom, strParam
    
    Select Case LCase(strCom)
        Case "version"
            PutData DataIn, strColor & "04" & "[" & strName & "] " & strCom
            PutData DataIn, strColor & "05" & strBold & strNick & strBold & " has just requested your client version"
            Client.CTCPReply strNick, "VERSION projectIRC for Windows"
        Case "ping"
            strTemp = RightOf(strData, " ")
            PutData DataIn, strColor & "04" & "[" & strName & "] " & strCom
            PutData DataIn, strColor & "05" & strBold & strNick & strBold & " has just pinged you"
            Client.CTCPReply strNick, "PING " & strTemp
        Case "action"
            PutData DataIn, strColor & "06" & strNick & " " & strParam
        Case "dcc"
            Seperate strData, " ", strTemp, strData
            strArgs = Split(strData, " ")
            
            Select Case LCase(strArgs(0))
                Case "send"
                    dccinfo.File = strArgs(1)
                    dccinfo.IP = strArgs(2)
                    
                    dccinfo.Port = strArgs(3)
                    dccinfo.Size = CLng(strArgs(4))
                    'MsgBox dccinfo.Size
                    dccinfo.Nick = strNick
                    dccinfo.type = 2
                    inttemp = NewDCCSend(dccinfo)
                    TimeOut 0.5
                    DCCSends(inttemp).sock.Connect
            End Select
        Case Else
            PutData DataIn, strColor & "04" & "[" & strName & "] " & strCom
    End Select
End Sub



Public Sub RemoveMode(strMode As String)
    Dim i As Integer, j As Integer
    
    For i = 1 To Toolbar1.Buttons.Count
        If Toolbar1.Buttons.Item(i).Key = strMode Then
            Toolbar1.Buttons.Item(i).Value = tbrUnpressed
        End If
    Next i
    
    For i = 1 To intModes
        'MsgBox Modes(i).mode & "~" & strMode & ".."
        If Modes(i).mode = strMode Then
            Modes(i).mode = ""
            For j = i To intModes - 1
                Modes(j) = Modes(j + 1)
            Next j
            intModes = intModes - 1
            On Error Resume Next
            ReDim Preserve Modes(1 To intModes) As typMode
            Update
            Exit Sub
        End If
    Next i
End Sub

Public Sub RemoveNick(strNick As String)
    Dim i As Integer, j As Integer, strTemp As String
    
    For i = 1 To intNicks
        If Nicks(i).Nick = strNick Then
            For j = i To intNicks - 1
                Nicks(j) = Nicks(j + 1)
            Next j
            intNicks = intNicks - 1
            On Error Resume Next
            ReDim Preserve Nicks(1 To intNicks) As Nick
            
            For j = 0 To lstNicks.ListCount - 1
                strTemp = lstNicks.List(j)
                strTemp = Replace(strTemp, "@", "")
                strTemp = Replace(strTemp, "+", "")
                strTemp = Replace(strTemp, "%", "")
                If strTemp = strNick Then
                    lstNicks.RemoveItem j
                    intUserCount = intUserCount - 1
                    Update
                    Exit Sub
                End If
            Next j
            Exit Sub
        End If
    Next i
End Sub

Public Sub SetHelper(strNick As String, bWhich As Boolean)
    Dim i As Integer, bInd As Integer
    
    For i = 1 To intNicks
        If Nicks(i).Nick = strNick Then
            Nicks(i).helper = bWhich
            bInd = i
            Exit For
        End If
    Next i
    For i = 0 To lstNicks.ListCount - 1
        If RealNick(lstNicks.List(i)) = strNick Then
            lstNicks.RemoveItem i
            lstNicks.AddItem DisplayNick(Nicks(bInd))
            Exit For
        End If
    Next i
End Sub

Public Sub SetOp(strNick As String, bWhich As Boolean)
    Dim i As Integer, bInd As Integer
    
    For i = 1 To intNicks
        If Nicks(i).Nick = strNick Then
            Nicks(i).op = bWhich
            bInd = i
            Exit For
        End If
    Next i
    For i = 0 To lstNicks.ListCount - 1
        If RealNick(lstNicks.List(i)) = strNick Then
            lstNicks.RemoveItem i
            lstNicks.AddItem DisplayNick(Nicks(bInd))

            Exit For
        End If
    Next i
End Sub

Public Sub SetVoice(strNick As String, bWhich As Boolean)
    Dim i As Integer, bInd As Integer
    
    'MsgBox strNick & "~" & bWhich
    For i = 1 To intNicks
        If Nicks(i).Nick = strNick Then
            Nicks(i).voice = bWhich
            bInd = i
            'MsgBox Nicks(i).voice
            Exit For
        End If
    Next i
    For i = 0 To lstNicks.ListCount - 1
        If RealNick(lstNicks.List(i)) = strNick Then
            lstNicks.RemoveItem i
            lstNicks.AddItem DisplayNick(Nicks(bInd))

            Exit For
        End If
    Next i
End Sub

Sub Update()
    strMode = ModeString()
    Dim strExtra As String, strMd As String
    If intLimit <> 0 Then strExtra = strExtra & " " & CStr(intLimit)
    If strKey <> "" Then strExtra = strExtra & " " & strKey
    strMd = " [" & strMode & strExtra & "]"
    If strMd = " []" Then strMd = ""
    Me.Caption = strName & " [" & intUserCount & "]" & strMd
End Sub

Private Sub DataIn_Change()
    If Client.ActiveForm.Caption = Me.Caption Then
        newBuffer = False
    Else
        newBuffer = True
    End If
    Client.DrawToolbar
End Sub

Private Sub DataIn_DblClick()
    Dim txt As String
    txt = strLink
    
    If LCase(Left(txt, 7)) = "http://" Or _
       LCase(Left(txt, 6)) = "ftp://" Or _
       LCase(Left(txt, 7)) = "mailto:" Or _
       LCase(Left(txt, 4)) = "www." Or _
       LCase(Right(txt, 5)) = ".html" Or _
       LCase(Right(txt, 4)) = ".htm" _
    Then
        ShellExecute 0, "open", txt, "", "", 0
    ElseIf InChannel(txt) Then
        DoEvents
        NewQuery txt, ""
        strLink = txt
    ElseIf Left(txt, 1) = "#" Then
        Dim inttemp As Integer
        inttemp = GetChanIndex(txt)
        If inttemp = -1 Then Client.SendData "JOIN " & txt
    ElseIf LCase(Left(txt, 4)) = "irc." Then
        DoConnect txt
    Else
        bLink = False
    End If
End Sub

Private Sub DataIn_MouseMove(Button As Integer, Shift As Integer, x As Single, Y As Single)
    Dim txt As String

    txt = RichWordOver(DataIn, x, Y)
'    If txt <> "" Then MsgBox "~" & txt & "~"
    If LCase(Left(txt, 7)) = "http://" Or _
       LCase(Left(txt, 6)) = "ftp://" Or _
       LCase(Left(txt, 7)) = "mailto:" Or _
       LCase(Left(txt, 4)) = "www." Or _
       LCase(Right(txt, 5)) = ".html" Or _
       LCase(Right(txt, 4)) = ".htm" _
    Then
        If DataIn.MousePointer <> 99 Then DataIn.MousePointer = 99
    ElseIf InChannel(txt) Then
        If DataIn.MousePointer <> 99 Then DataIn.MousePointer = 99
    ElseIf Left(txt, 1) = "#" Then
        If DataIn.MousePointer <> 99 Then DataIn.MousePointer = 99
    ElseIf LCase(Left(txt, 4)) = "irc." Then
        If DataIn.MousePointer <> 99 Then DataIn.MousePointer = 99
    Else
        If DataIn.MousePointer <> 1 Then DataIn.MousePointer = 1
    End If

End Sub


Private Sub DataIn_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
    DoEvents
    Dim txt As String

    txt = RichWordOver(DataIn, x, Y)
'    If txt <> "" Then MsgBox "~" & txt & "~"
    strLink = txt
End Sub

Private Sub DataOut_KeyDown(KeyCode As Integer, Shift As Integer)
    If Shift = 2 Then bControl = True   'control
    
    If KeyCode = 38 Then    'UP KEY!
        If intCurHist <= 1 Then Beep: Exit Sub
        intCurHist = intCurHist - 1
        DataOut.Text = textHistory.Item(intCurHist)
        KeyCode = 0
    ElseIf KeyCode = 40 Then    'down key!
        If intCurHist >= textHistory.Count Or intCurHist = -1 Then Beep: Exit Sub
        intCurHist = intCurHist + 1
        DataOut.Text = textHistory.Item(intCurHist)
        KeyCode = 0
    End If
End Sub

Private Sub DataOut_KeyPress(KeyAscii As Integer)
    On Error Resume Next
    If KeyAscii = 13 Then
        If DataOut.Text = "" Then Exit Sub
        KeyAscii = 0
        
        If Left(DataOut.Text, 1) = "/" Then
            Client.SendData GetAlias(strName, RightR(DataOut.Text, 1))
            If Me.Tag = "PARTNOW" Then
                Me.Tag = "NOPART"
                Unload Me
                Exit Sub
            End If
        Else
            Client.SendData "PRIVMSG " & strName & " :" & DataOut.Text
            PutData DataIn, "" & strMyNick & " : " & Chr(9) & DataOut.Text
        End If
        DataOut.SelColor = lngForeColor
        DoHistory DataOut.Text
        DataOut.Text = ""
    End If
    
    If bControl Then
        'MsgBox KeyAscii
        If KeyAscii = 11 Then
            DataOut.SelText = strColor
        ElseIf KeyAscii = 2 Then
            DataOut.SelText = strBold
        ElseIf KeyAscii = 21 Then
            DataOut.SelText = strUnderline
        ElseIf KeyAscii = 18 Then
            DataOut.SelText = strReverse
        End If
    End If
    
End Sub


Private Sub DataOut_KeyUp(KeyCode As Integer, Shift As Integer)
    If Shift = 2 Then bControl = False   'control
End Sub

Private Sub DataOut_MouseDown(Button As Integer, Shift As Integer, x As Single, Y As Single)
    bdown = True
End Sub

Private Sub DataOut_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
    bdown = True
End Sub


Private Sub Form_Activate()
    DataOut.SetFocus
    intCurHist = -1
    newBuffer = False
    Dim i As Integer
    i = GetWindowIndex(strName)
    SetWinFocus i
    Client.intActive = i
    Client.intHover = -1
    'MsgBox i
    DoEvents
    Client.DrawToolbar
End Sub

Private Sub Form_GotFocus()
    DataOut.SetFocus
End Sub

Private Sub Form_Load()
    Client.DrawToolbar
    DataIn.BackColor = lngBackColor
    DataOut.BackColor = lngBackColor
    lstNicks.BackColor = lngBackColor
    rtbTopic.BackColor = lngBackColor
    rtbTopic.SelColor = lngForeColor
    DataOut.SelColor = lngForeColor
    lstNicks.ForeColor = lngForeColor
    
    '* Set the colors straight!!
    Me.BackColor = lngRightColor
    shpLeftC.BackColor = lngLeftColor
    shpTopic.BorderColor = lngLeftColor
    picTopic.BackColor = lngBackColor
    shpDI.BorderColor = lngLeftColor
    rtbTopic.SelColor = lngForeColor
    picFlat.BackColor = lngLeftColor
    Me.Visible = True
    DoEvents
    
    Dim strTemp As String, strPos As String, strCPos As String, strLst() As String
    strTemp = strName
    
    With Me
        strCPos = .Left & "," & _
                 .Top & "," & _
                 .Width & "," & _
                 .Height
    End With

    strPos = GetINI(winINI, "pos", "*" & strTemp, "-1,-1,-1,-1")
    If strPos = "-1,-1,-1,-1" Then
    '    MsgBox "The active window(" & strTemp & ")'s position has not yet been remembered, please remember it first", vbInformation
        Exit Sub
    End If
    
    strLst = Split(strPos, ",")
    
    On Error Resume Next
    Me.Move CInt(strLst(0)), CInt(strLst(1)), CInt(strLst(2)), CInt(strLst(3))


End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    
    Cancel = 1
    If Me.Tag <> "NOPART" Then
        Client.SendData "PART " & strName & " :closed channel"
    End If
    Me.Tag = ""
    
    strName = ""
    Me.Caption = ""
    lblName = ""
    strMode = ""
    intModes = 0
    intUserCount = 0
    intCurHist = 0
    
    Dim i As Integer
    For i = 1 To textHistory.Count
        textHistory.Remove 1
    Next i
    
    strKey = ""
    intLimit = 0
    On Error Resume Next
    If Val(Me.Tag) = intChannels Then intChannels = intChannels - 1
    Unload Channels(Me.Tag)
    Unload Me
    Cancel = 0
    Client.DrawToolbar
End Sub

Private Sub Form_Resize()
    If Me.WindowState = vbMinimized Then Exit Sub
    If Me.Width < 4500 Then Me.Width = 4500
    If Me.Height < 2500 Then Me.Height = 2500
    
    shpTopic.Width = Me.ScaleWidth - 2190
    picTopic.Width = shpTopic.Width - 50
    rtbTopic.Width = picTopic.Width + 150
    shpDI.Width = Me.ScaleWidth - 2100
    shpDI.Height = Me.ScaleHeight - 550
    picFlat.Width = Me.ScaleWidth - 650
    picFlat.Height = Me.ScaleHeight - 600
    DataIn.Width = Me.ScaleWidth - 2320
    DataIn.Height = Me.ScaleHeight - 870
    DataOut.Width = Me.ScaleWidth - 560
    picNicks.Left = Me.ScaleWidth - 2290
    picNicks.Height = DataIn.Height
    lstNicks.Height = DataIn.Height + 80
    picDO.Top = Me.ScaleHeight - 840
    picDO.Width = Me.ScaleWidth - 150
    shpLeftC.Height = Me.ScaleHeight + 25
    Toolbar.Left = Me.ScaleWidth - 850
    Toolbar1.Left = Me.ScaleWidth - 390
End Sub


Private Sub lstNicks_DblClick()
    NewQuery RealNick(lstNicks.List(lstNicks.ListIndex)), ""
End Sub


Private Sub lstNicks_MouseDown(Button As Integer, Shift As Integer, x As Single, Y As Single)
    If Button = vbRightButton Then
        
        Dim strNick As String
        strNick = RealNick(lstNicks.List(lstNicks.ListIndex))
        
        If isOp(strNick) Then
            Client.mnu_nicks_op.Caption = "De&Op"
        Else
            Client.mnu_nicks_op.Caption = "&Op"
        End If
        If isVoice(strNick) Then
            Client.mnu_nicks_voice.Caption = "&Voice"
        Else
            Client.mnu_nicks_voice.Caption = "De&Voice"
        End If
        If isHalfOp(strNick) Then
            Client.mnu_nicks_halfop.Caption = "De&HalfOp"
        Else
            Client.mnu_nicks_halfop.Caption = "&HalfOp"
        End If
        
        PopupMenu Client.mnu_nicks
    End If
End Sub


Private Sub rtbTopic_Change()
    rtbTopic.ToolTipText = rtbTopic.Text
End Sub

Private Sub rtbTopic_KeyDown(KeyCode As Integer, Shift As Integer)
    If rtbTopic.Tag = "locked" Then
        If KeyCode = 8 Then KeyCode = 0
        If KeyCode = 46 Then KeyCode = 0
        
        'MsgBox KeyCode
    End If
End Sub

Private Sub rtbTopic_KeyPress(KeyAscii As Integer)
    If rtbTopic.Tag = "locked" Then KeyAscii = 0: Exit Sub
    If KeyAscii = 13 Then
        Client.SendData "TOPIC " & strName & " :" & rtbTopic.Text
        KeyAscii = 0
    End If
    
End Sub


Private Sub Toolbar1_ButtonClick(ByVal Button As MSComctlLib.Button)
    Dim strNick As String
    strNick = RealNick(lstNicks.List(lstNicks.ListIndex))
    
    If isOp(strNick) Then
        Client.mnu_nicks_op.Caption = "De&Op"
    Else
        Client.mnu_nicks_op.Caption = "&Op"
    End If
    If isVoice(strNick) Then
        Client.mnu_nicks_voice.Caption = "&Voice"
    Else
        Client.mnu_nicks_voice.Caption = "De&Voice"
    End If
    If isHalfOp(strNick) Then
        Client.mnu_nicks_halfop.Caption = "De&HalfOp"
    Else
        Client.mnu_nicks_halfop.Caption = "&HalfOp"
    End If
    

    Select Case Button.Index
        Case 1:              Client.mnu_nicks_op_Click
        Case 2:              Client.mnu_nicks_halfop_Click
        Case 3:              Client.mnu_nicks_voice_Click
        Case Else:
            Dim strPlus As String, strAppend As String
            strAppend = ""
            strPlus = "-"
            
            If Button.Value = tbrPressed Then strPlus = "+"
            
            
            If Button.Key = "k" And Button.Value = tbrPressed Then
                strAppend = InputBox("Please enter the new channel key:", "Change key", strKey)
                If strAppend = "" Then Exit Sub
                
            ElseIf Button.Key = "l" And Button.Value = tbrPressed Then
                strAppend = InputBox("Enter the new channels user limit:", "Change user limit", intLimit)
                If strAppend = "" Then Exit Sub
                'Button.Value = tbrUnpressed
            ElseIf Button.Key = "k" And Button.Value = tbrUnpressed Then
                strAppend = strKey
                'Button.Value = tbrPressed
            End If
            If Button.Value = tbrPressed Then
                Button.Value = tbrUnpressed
            Else
                Button.Value = tbrPressed
            End If
            
            If strAppend <> "" Then strAppend = " " & strAppend
            Client.SendData "MODE " & strName & " " & strPlus & Button.Key & strAppend
    End Select
End Sub

