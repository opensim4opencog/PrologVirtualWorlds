VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Begin VB.Form Status 
   Caption         =   "Status"
   ClientHeight    =   3705
   ClientLeft      =   1680
   ClientTop       =   5865
   ClientWidth     =   6600
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "frmStatus.frx":0000
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   NegotiateMenus  =   0   'False
   ScaleHeight     =   3705
   ScaleWidth      =   6600
   Begin MSComctlLib.ImageList ImageList1 
      Left            =   3630
      Top             =   225
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   2
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmStatus.frx":058A
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmStatus.frx":09DE
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin VB.PictureBox picFlat 
      BackColor       =   &H00800000&
      BorderStyle     =   0  'None
      Height          =   3195
      Left            =   150
      ScaleHeight     =   3195
      ScaleWidth      =   6330
      TabIndex        =   2
      Top             =   375
      Width           =   6330
      Begin VB.PictureBox picDO 
         BorderStyle     =   0  'None
         Height          =   240
         Left            =   0
         ScaleHeight     =   240
         ScaleWidth      =   6360
         TabIndex        =   4
         Top             =   2970
         Width           =   6360
         Begin RichTextLib.RichTextBox DataOut 
            Height          =   390
            Left            =   -45
            TabIndex        =   5
            Top             =   -45
            Width           =   6510
            _ExtentX        =   11483
            _ExtentY        =   688
            _Version        =   393217
            MultiLine       =   0   'False
            TextRTF         =   $"frmStatus.frx":0E32
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
         TabIndex        =   3
         Top             =   15
         Width           =   6450
         _ExtentX        =   11377
         _ExtentY        =   5159
         _Version        =   393217
         BorderStyle     =   0
         HideSelection   =   0   'False
         ReadOnly        =   -1  'True
         ScrollBars      =   2
         Appearance      =   0
         AutoVerbMenu    =   -1  'True
         TextRTF         =   $"frmStatus.frx":0EBC
         MouseIcon       =   "frmStatus.frx":0F46
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
   Begin MSComctlLib.Toolbar Toolbar 
      Height          =   330
      Left            =   5790
      TabIndex        =   6
      Top             =   30
      Width           =   690
      _ExtentX        =   1217
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
   Begin VB.Shape shpTBorder 
      BorderColor     =   &H00800000&
      BorderWidth     =   2
      Height          =   405
      Left            =   5775
      Top             =   15
      Width           =   735
   End
   Begin VB.Shape shpDI 
      BorderColor     =   &H00800000&
      BorderWidth     =   2
      Height          =   3225
      Left            =   1635
      Top             =   375
      Width           =   4875
   End
   Begin VB.Label lblServer 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "not connected"
      Height          =   195
      Left            =   1740
      TabIndex        =   1
      Top             =   60
      Width           =   1035
   End
   Begin VB.Label lbl1 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Server Status"
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
      Left            =   75
      TabIndex        =   0
      Top             =   75
      Width           =   1425
   End
   Begin VB.Shape shpLeftC 
      BackColor       =   &H00800000&
      BackStyle       =   1  'Opaque
      BorderStyle     =   0  'Transparent
      Height          =   3930
      Left            =   -15
      Top             =   -15
      Width           =   1650
   End
End
Attribute VB_Name = "Status"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public bControl As Boolean
Public newBuffer As Boolean
Private Sub Command1_Click()

End Sub


Sub Update()
    Dim mode As String
    mode = "[" & Client.ModeString & "] "
    If mode = "[] " Then mode = ""
    
    If Client.sock.State = 0 Then
        Status.Caption = "Status"
    Else
        Status.Caption = "Status : " & strMyNick & " " & mode & "on " & Client.sock.RemoteHost
    End If
End Sub


Private Sub DataIn_Change()
    DataIn.SelStart = Len(DataIn.Text)
    On Error Resume Next
    If Client.ActiveForm.Caption = Me.Caption Then
        newBuffer = False
    Else
        newBuffer = True
    End If
    Client.DrawToolbar
End Sub

Private Sub DataIn_GotFocus()
    Dim lngRet As Long
    lngRet = HideCaret(DataIn.hwnd)
    DataOut.SetFocus
End Sub

Private Sub DataIn_LostFocus()
    Dim lngRet As Long
    lngRet = ShowCaret(DataIn.hwnd)
End Sub


Private Sub DataIn_MouseDown(Button As Integer, Shift As Integer, x As Single, Y As Single)
    Dim lngRet As Long
    lngRet = HideCaret(DataIn.hwnd)
    
    DoEvents
    Dim txt As String

    txt = RichWordOver(DataIn, x, Y)
'    If txt <> "" Then MsgBox "~" & txt & "~"
    If LCase(Left(txt, 7)) = "http://" Or _
       LCase(Left(txt, 6)) = "ftp://" Or _
       LCase(Left(txt, 7)) = "mailto:" _
    Then
        If Button = 2 Then
            
            'MsgBox "~" & txt
            ShellExecute 0, "open", txt, "", "", 0
        End If
    End If
End Sub

Private Sub DataIn_MouseMove(Button As Integer, Shift As Integer, x As Single, Y As Single)
    Dim txt As String

    txt = RichWordOver(DataIn, x, Y)
'    If txt <> "" Then MsgBox "~" & txt & "~"
    If LCase(Left(txt, 7)) = "http://" Or _
       LCase(Left(txt, 6)) = "ftp://" Or _
       LCase(Left(txt, 7)) = "mailto:" _
    Then
        If DataIn.MousePointer <> 99 Then DataIn.MousePointer = 99
    Else
        If DataIn.MousePointer <> 1 Then DataIn.MousePointer = 1
    End If
End Sub


Private Sub DataIn_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
    Dim lngRet As Long
    lngRet = HideCaret(DataIn.hwnd)

End Sub

Private Sub DataOut_GotFocus()
    DataOut.SelColor = lngForeColor
End Sub

Private Sub DataOut_KeyDown(KeyCode As Integer, Shift As Integer)
    If Shift = 2 Then bControl = True   'control

End Sub

Private Sub DataOut_KeyPress(KeyAscii As Integer)
    If KeyAscii = 13 Then
        If DataOut.Text = "" Then Exit Sub
        KeyAscii = 0
        If Left(DataOut.Text, 1) = "/" Then
            Client.SendData GetAlias("", RightR(DataOut.Text, 1))
        Else
            Client.SendData DataOut.Text
        End If
        DataOut.Text = ""
        DataOut.SelColor = lngForeColor
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


Private Sub Form_Activate()
    Dim lngRet As Long
    lngRet = HideCaret(DataIn.hwnd)
    
    On Error Resume Next
    DataIn.BackColor = lngBackColor
    DataOut.BackColor = lngBackColor
    DataOut.SetFocus
    newBuffer = False
    Client.mnu_View_Status.Checked = True
    SetWinFocus 1
    Client.intHover = -1
    Client.intActive = 1
    Client.DrawToolbar
End Sub

Private Sub Form_GotFocus()
    If Me.Visible Then DataOut.SetFocus
End Sub

Private Sub Form_Load()
    
    strBold = Chr(bold)
    strUnderline = Chr(underline)
    strColor = Chr(Color)
    strReverse = Chr(REVERSE)
    strAction = Chr(ACTION)
    
'    DataIn.SelRTF = "{\rtf1\ansi\ansicpg1252\deff0\deftab720{\fonttbl{\f0\fswiss " & strFont & ";}{\f1\froman\fcharset2 Symbol;}{\f2\fswiss " & strFont & ";}{\f3\fswiss " & strFont & ";}}" & vbCrLf & _
                    "{\colortbl\red0\green0\blue0;\red0\green0\blue255;\red255\green0\blue0;\red0\green0\blue127;\red0\green127\blue0;\red255\green255\blue255;\red127\green0\blue127;\red255\green127\blue0;\red255\green255\blue0;\red127\green0\blue0;\red0\green148\blue144;\red0\green255\blue255;\red0\green255\blue0;\red92\green92\blue92;\red184\green184\blue184;\red255\green0\blue255;}" & vbCrLf & _
                    "\deflang1033\pard\plain\f6\fs18  }"
    
    
    PutData Status.DataIn, "Welcome to " & strBold & strColor & "12" & "Prolog Virtual Worlds" & strColor & "!"
    PutData Status.DataIn, "Prolog Virtual Worlds version " & strColor & "04" & "1" & strColor & " build " & strColor & "04" & App.Revision & strColor
    
    TimeOut 0.01
    
    '* Set the colors straight!!
    Me.BackColor = lngRightColor
    shpLeftC.BackColor = lngLeftColor
    shpDI.BorderColor = lngLeftColor
    picFlat.BackColor = lngLeftColor
    DataOut.SelColor = lngForeColor
    lbl1.ForeColor = lngBackColor
    shpTBorder.BorderColor = lngLeftColor
    
    '* Set the font!
    On Error Resume Next
    Status.DataIn.Font.name = strFontName
    Status.DataIn.SelFontName = strFontName
    
    '* If connect on load, do it
    If bConOnLoad Then
        Call Client.mnu_File_Connect_Click
    End If
    
    'If BuddyList.Visible = False Then
    '    bShowBL = True
    '    Load BuddyList
    '    TimeOut 0.01
    'End If
    Me.Visible = True
    DoEvents
    Client.mnu_view_ResetAWPos_Click
    
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    Cancel = 1
    Me.Hide
End Sub

Private Sub Form_Resize()
    If Status.WindowState = vbMinimized Then Exit Sub
    If Status.Width < 4500 Then Me.Width = 4500
    If Status.Height < 2500 Then Status.Height = 2500
    
    '* Resizing.. oh boy
    If intIface = IFT_FANCY Then    'fancy interface
        shpDI.Width = Status.ScaleWidth - 1800
        shpDI.Height = Status.ScaleHeight - 570
        picFlat.Width = Status.ScaleWidth - 330
        picFlat.Height = Status.ScaleHeight - 600
        DataIn.Width = Status.ScaleWidth - 350
        DataIn.Height = Status.ScaleHeight - 870
        DataOut.Width = Status.ScaleWidth - 180
        picDO.Top = Status.ScaleHeight - 830
        picDO.Width = Status.ScaleWidth - 350
        shpLeftC.Height = Status.ScaleHeight + 25
        Toolbar.Left = Status.ScaleWidth - 890
        shpTBorder.Left = Status.ScaleWidth - 900
    Else    'simple
        shpDI.Top = 10
        shpDI.Width = Status.ScaleWidth - 1630
        shpDI.Height = Status.ScaleHeight - 10
        picFlat.Width = Status.ScaleWidth - 0
        DataIn.Width = Status.ScaleWidth - 0
        picFlat.Height = Status.ScaleHeight + 10
        DataIn.Height = Status.ScaleHeight - 270
        picFlat.Top = -10
        picFlat.Left = 0
        DataOut.Width = Status.ScaleWidth + 130
        DataOut.Top = 0
        picDO.Top = Status.ScaleHeight - 270
        picDO.Height = 430
        picDO.Width = Status.ScaleWidth - 0
        shpLeftC.Height = Status.ScaleHeight + 25
        Toolbar.Left = Status.ScaleWidth - 890
        shpTBorder.Left = Status.ScaleWidth - 900
    End If
End Sub


Private Sub sock_Error(ByVal Number As Integer, Description As String, ByVal Scode As Long, ByVal Source As String, ByVal HelpFile As String, ByVal HelpContext As Long, CancelDisplay As Boolean)

End Sub


Private Sub Toolbar_ButtonClick(ByVal Button As MSComctlLib.Button)
    Select Case Button.Index
        Case 1
            '* Connect
            Client.mnu_File_Connect_Click
        Case 2
            '* Disconnect
            Call Client.mnu_File_Disconnect_Click
    End Select
End Sub


