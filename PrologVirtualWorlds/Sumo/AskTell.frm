VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Begin VB.Form Discourse 
   Caption         =   "Discourse Window"
   ClientHeight    =   4020
   ClientLeft      =   9180
   ClientTop       =   5895
   ClientWidth     =   9990
   Icon            =   "AskTell.frx":0000
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   4020
   ScaleWidth      =   9990
   Begin RichTextLib.RichTextBox OutputWindow 
      Height          =   795
      Left            =   390
      TabIndex        =   7
      Top             =   465
      Width           =   4140
      _ExtentX        =   7303
      _ExtentY        =   1402
      _Version        =   393217
      Enabled         =   -1  'True
      ScrollBars      =   3
      TextRTF         =   $"AskTell.frx":0442
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Verdana"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
   Begin VB.CommandButton cmdFile 
      Caption         =   "File"
      Height          =   360
      Left            =   3165
      TabIndex        =   6
      Top             =   2520
      Width           =   990
   End
   Begin VB.ComboBox OptCombo 
      Height          =   315
      Left            =   4425
      Style           =   2  'Dropdown List
      TabIndex        =   5
      Top             =   2535
      Width           =   1965
   End
   Begin VB.ComboBox DefaultContext 
      Height          =   315
      ItemData        =   "AskTell.frx":04C6
      Left            =   6450
      List            =   "AskTell.frx":04CD
      Style           =   2  'Dropdown List
      TabIndex        =   4
      Top             =   2895
      Visible         =   0   'False
      Width           =   1500
   End
   Begin VB.CommandButton cmdCommand 
      Caption         =   "Command"
      Height          =   375
      Left            =   7605
      TabIndex        =   3
      Top             =   2505
      Width           =   1005
   End
   Begin VB.CommandButton cmdRetract 
      Caption         =   "Retract"
      Height          =   375
      Left            =   2130
      TabIndex        =   2
      Top             =   2520
      Width           =   975
   End
   Begin VB.CommandButton cmdTell 
      Caption         =   "Tell"
      Height          =   375
      Left            =   1095
      TabIndex        =   1
      Top             =   2505
      Width           =   975
   End
   Begin VB.CommandButton cmdAsk 
      Caption         =   "Ask"
      Height          =   375
      Left            =   60
      TabIndex        =   0
      Top             =   2505
      Width           =   975
   End
End
Attribute VB_Name = "Discourse"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False



Private Sub Form_Load()
    OutputWindow.Top = 5
    OutputWindow.Left = 5
    Form_Resize
    OptionSwitches = "[]"
    DefaultContext = "default_context"
    a = Split(AllOptions, vbCrLf)
    For i = 0 To UBound(a) - 1
        fAskTell.OptCombo.AddItem Split(a(i), ":")(0), i
        OptionString(i) = Split(a(i), ":")(1)
    
    Next
    fAskTell.OptCombo.Text = Split(AllOptions, ":")(0)
    fAskTell.DefaultContext.ListIndex = 0
    fAskTell.Show
    fAskTell.SetFocus
End Sub


Private Sub Form_Resize()
 On Error Resume Next
    OutputWindow.Top = 5
    OutputWindow.Left = 5
    OutputWindow.Width = Me.Width - 90
    OutputWindow.Height = Me.Height - 900
    Top2 = Me.Height - 820
    cmdAsk.Top = Top2
    cmdTell.Top = Top2
    cmdRetract.Top = Top2
    cmdCommand.Top = Top2
    cmdFile.Top = Top2
    OptCombo.Top = Top2
    DefaultContext.Top = Top2
End Sub

Private Sub cmdFile_Click()
    If OptCombo.Text = "Prolog" Then Call fMainForm.mnuPrologConsult_Click: Exit Sub
    Dim sFile As String
On Error Resume Next

    With fMainForm.dlgCommonDialog
        .DialogTitle = "Consult a " & OptCombo.Text & " file.."
        .CancelError = False
        'ToDo: set the flags and attributes of the common dialog control
        .Filter = "Knowledge Source Files (*.KIF;*.P)|*.P;*.KIF"
        .ShowOpen
        If Len(.FileName) = 0 Then
            Exit Sub
        End If
        sFile = .FileName

'    Stop
    FileSystem.FileCopy .FileName, "C:\cygwin\tks\sigma\inference_engine\resources\" & .FileTitle
    Stop
    CallProlog "ua_read('resources/" & .FileTitle & "'," & OptionSwitches & ")."
    End With
    
    
End Sub

Private Sub cmdAsk_Click()
    Dim KIF As String
    KIF = SafeText(OutputWindow.Text)
    If OptCombo.Text = "Prolog" Then CallProlog "((" & KIF & ")).": Exit Sub
'    Stop
    While KIF <> ""
        F = Head(KIF)
        CallProlog "ua_ask(" & Chr(34) & F & Chr(34) & "," & DefaultContext & "," & OptionSwitches & ")."
    Wend
End Sub
Private Sub cmdTell_Click()
    Dim KIF As String
    KIF = SafeText(OutputWindow.Text)
    If OptCombo.Text = "Prolog" Then CallProlog "assert((" & KIF & ")).": Exit Sub
    While KIF <> ""
        F = Head(KIF)
    CallProlog "ua_tell(" & Chr(34) & F & Chr(34) & "," & DefaultContext & ",_," & OptionSwitches & ")."
    Wend
End Sub
Private Sub cmdRetract_Click()
    Dim KIF As String
    KIF = SafeText(OutputWindow.Text)
    If OptCombo.Text = "Prolog" Then CallProlog "retractall((" & KIF & ")).": Exit Sub
    While KIF <> ""
        F = Head(KIF)
    CallProlog "ua_retract(" & Chr(34) & F & Chr(34) & "," & DefaultContext & ",_," & OptionSwitches & ")."
    Wend
End Sub
Private Sub cmdCommand_Click()
    Dim KIF As String
    KIF = SafeText(OutputWindow.Text)
    If OptCombo.Text = "Prolog" Then CallProlog "((" & KIF & ")).": Exit Sub
    While KIF <> ""
        F = Head(KIF)
    CallProlog "ua_command(" & Chr(34) & F & Chr(34) & "," & OptionSwitches & ")."
    Wend
End Sub

Private Sub OutputWindow_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
   Me.Show
   Me.OutputWindow.SetFocus
   If Button = 2 Then   ' Check if right mouse button
                        ' was clicked.
      PopupMenu fMainForm.mnuEdit   ' Display the File menu as a
                        ' pop-up menu.
   End If
End Sub

Private Sub OptCombo_Change()
    On Error Resume Next
    OptionSwitches = Split(Split(AllOptions, OptCombo.Text & ":")(1), vbCrLf)(0)
End Sub

Private Sub OptCombo_Click()
    On Error Resume Next
    OptionSwitches = Split(Split(AllOptions, OptCombo.Text & ":")(1), vbCrLf)(0)
End Sub
