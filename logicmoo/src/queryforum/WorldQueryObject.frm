VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Begin VB.Form QueryObject 
   AutoRedraw      =   -1  'True
   Caption         =   "Initalizing..."
   ClientHeight    =   9300
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   10665
   Icon            =   "WorldQueryObject.frx":0000
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   9300
   ScaleWidth      =   10665
   ShowInTaskbar   =   0   'False
   WindowState     =   2  'Maximized
   Begin VB.CommandButton cmdSelectResource 
      Caption         =   "Resource"
      Height          =   330
      Left            =   2160
      TabIndex        =   3
      Top             =   840
      Width           =   870
   End
   Begin VB.ComboBox txtDistributions 
      Height          =   315
      Left            =   -120
      TabIndex        =   9
      Text            =   "Distributions"
      Top             =   2520
      Width           =   1695
   End
   Begin VB.TextBox txtFriendlyName 
      Height          =   285
      Left            =   0
      TabIndex        =   8
      Text            =   "FriendlyName"
      Top             =   240
      Width           =   2055
   End
   Begin VB.ComboBox txtNativeLanguage 
      Height          =   315
      Left            =   0
      TabIndex        =   7
      Text            =   "NativeLanguage"
      Top             =   840
      Width           =   2055
   End
   Begin VB.ComboBox txtSubscriptions 
      Height          =   315
      Left            =   1680
      TabIndex        =   6
      Text            =   "Subscriptions"
      Top             =   2520
      Width           =   1455
   End
   Begin VB.ComboBox TargetObject 
      Height          =   315
      Left            =   -120
      TabIndex        =   5
      Text            =   "TargetObject"
      Top             =   2160
      Width           =   1695
   End
   Begin VB.ComboBox txtMessage 
      Height          =   315
      Left            =   1680
      TabIndex        =   4
      Text            =   "txtMessage"
      Top             =   2160
      Width           =   3015
   End
   Begin VB.CommandButton CancelButton 
      Caption         =   "Cancel"
      Height          =   330
      Left            =   2160
      TabIndex        =   2
      Top             =   435
      Width           =   870
   End
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Height          =   345
      Left            =   2160
      TabIndex        =   1
      Top             =   0
      Width           =   870
   End
   Begin VB.CommandButton CmdXMIT 
      Caption         =   "XMIT"
      Height          =   315
      Left            =   3240
      TabIndex        =   0
      Top             =   2520
      Width           =   870
   End
   Begin RichTextLib.RichTextBox OutputWindow 
      Height          =   435
      Left            =   -120
      TabIndex        =   10
      Top             =   1320
      Width           =   6360
      _ExtentX        =   11218
      _ExtentY        =   767
      _Version        =   393217
      Enabled         =   -1  'True
      ScrollBars      =   3
      TextRTF         =   $"WorldQueryObject.frx":030A
   End
   Begin VB.Label Label1 
      BackColor       =   &H80000000&
      Caption         =   "From:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   11.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   1
      Left            =   1680
      TabIndex        =   15
      Top             =   1800
      Width           =   855
   End
   Begin VB.Label Label1 
      BackColor       =   &H80000000&
      Caption         =   "Text To Say"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   11.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   2
      Left            =   3240
      TabIndex        =   14
      Top             =   1800
      Width           =   1335
   End
   Begin VB.Label Label1 
      BackColor       =   &H80000000&
      Caption         =   "To:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   11.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   0
      Left            =   0
      TabIndex        =   13
      Top             =   1800
      Width           =   1335
   End
   Begin VB.Label Label1 
      Caption         =   "Language"
      Height          =   375
      Index           =   3
      Left            =   0
      TabIndex        =   12
      Top             =   600
      Width           =   1815
   End
   Begin VB.Label Label1 
      Caption         =   "Name"
      Height          =   375
      Index           =   4
      Left            =   0
      TabIndex        =   11
      Top             =   0
      Width           =   1815
   End
End
Attribute VB_Name = "QueryObject"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
#Const DebugMode = 1
Public Resources As String

'Default Property Values:
Const m_def_BackColor = 0
Const m_def_Enabled = 0
Const m_def_BackStyle = 0
Const m_def_BorderStyle = 0

'Property Variables:
Dim m_BackColor As Long
Dim m_Enabled As Boolean
Dim m_BackStyle As Integer
Dim m_BorderStyle As Integer
'Dim m_Image As Picture

'Events

'Private WithEvents TheQueryObject As QueryActors.Agent
Public WithEvents TheBotConnection As AvatarInstance
Attribute TheBotConnection.VB_VarHelpID = -1

Private PrologObject As SWIProlog

Public LanguagesSpoken As VWSYSTEMLib.ipropertylist
Public LanguagesHeard As VWSYSTEMLib.ipropertylist
Public theWorldWalker


Private Sub cmdSelectResource_Click()
    PrologObject.Consult ("C:\msr\output\Cleanup.prolog")
End Sub


Private Sub Form_Load()
   ' Set TheQueryObject = New QueryActors.Agent
   ' Set TheBotConnection = New VWBotLib.BotConnection
    Set PrologObject = New SWIProlog
    PrologObject.Consult ("C:\msr\output\Cleanup.prolog")
End Sub

Public Sub OKButton_Click()
 On Error Resume Next
    If txtNativeLanguage = "" Then Exit Sub
    txtMessage = "I am " & txtFriendlyName.Text
 '   TheQueryObject.NativeLanguage = txtNativeLanguage.Text
 '   TheQueryObject.FriendlyName = txtFriendlyName.Text
    OKButton.Visible = False
    CancelButton.Visible = False
    Width = 9060:    Height = 3180
    

    Set TheBotConnection = New BotConnection

    
     TheBotConnection.ConnectMe "mbuild/scripting", "vscripter2"
     
     form_Resize
     
    Set theWorldWalker = New WorldWalker
End Sub


Private Sub OutputWindow_KeyPress(KeyAscii As Integer)
Dim t$, i%, LineNum%, TextRight As String, tr$
    If KeyAscii <> 13 Then Exit Sub
    t$ = Left(OutputWindow.Text, OutputWindow.SelStart)

    LineNum% = OutputWindow.GetLineFromChar(OutputWindow.SelStart)
    t$ = Replace(t$, vbCrLf, vbLf)
    t$ = Replace(t$, vbCr, vbLf)
    For i% = 1 To LineNum
       t$ = Mid(t$, InStr(t$, vbLf) + 1)
    Next i%
    If Len(t$) > 3 Then tr$ = PrologObject.Execute(t$)
    'TheQueryObject.NativeQuery = t$

    'TextRight = LCase(TheQueryObject.NativeLanguage) & "(" & tr$ & ")"
    TextRight = tr$
    
    SendToOutputWindow (TextRight)
End Sub


    
Public Sub SendToOutputWindow(ByVal TextRight)
    TextRight = Trim(TextRight) & vbCrLf
On Error Resume Next
'   If Len(OutputWindow.Text) > 2000 Then OutputWindow.Text = Right(OutputWindow.Text, 1500)
    OutputWindow.Text = VBA.Left$(OutputWindow.Text, OutputWindow.SelStart + 1) & TextRight & VBA.Mid$(OutputWindow.Text, OutputWindow.SelStart + 1)
    OutputWindow.SelStart = Len(OutputWindow.Text)
    OutputWindow.SelStart = OutputWindow.SelStart + Len(TextRight) + 1
End Sub


Private Sub OutputWindow_SelChange()
    MainForm.tbToolBar.Buttons("Bold").value = IIf(OutputWindow.SelBold, tbrPressed, tbrUnpressed)
    MainForm.tbToolBar.Buttons("Italic").value = IIf(OutputWindow.SelItalic, tbrPressed, tbrUnpressed)
    MainForm.tbToolBar.Buttons("Underline").value = IIf(OutputWindow.SelUnderline, tbrPressed, tbrUnpressed)
    MainForm.tbToolBar.Buttons("Align Left").value = IIf(OutputWindow.SelAlignment = rtfLeft, tbrPressed, tbrUnpressed)
    MainForm.tbToolBar.Buttons("Center").value = IIf(OutputWindow.SelAlignment = rtfCenter, tbrPressed, tbrUnpressed)
    MainForm.tbToolBar.Buttons("Align Right").value = IIf(OutputWindow.SelAlignment = rtfRight, tbrPressed, tbrUnpressed)
End Sub

Private Sub form_Resize()
    On Error Resume Next
    If txtNativeLanguage = "" Then Exit Sub
    
    Dim i As Long
    
    'Debug.Print "Resize: " & Width & ", " & Height
    OutputWindow.Width = Width - 150
    OutputWindow.Left = 0
    OutputWindow.Top = 0
    
    OutputWindow.Height = Int(ScaleHeight - txtNativeLanguage.Height - txtMessage.Height - Label1(0).Height)
    Label1(0).Top = OutputWindow.Height + 20
    Label1(1).Top = OutputWindow.Height + 20
    Label1(2).Top = OutputWindow.Height + 20
    Label1(0).Left = 0
    Label1(1).Left = Int(Width * 0.25)
    Label1(2).Left = Int(Width * 0.5)
    Label1(0).Width = Int(Width * 0.25)
    Label1(1).Width = Int(Width * 0.25)
    Label1(2).Width = Int(Width * 0.25)
    TargetObject.Left = Label1(0).Left
    txtFriendlyName.Left = Label1(1).Left
    txtMessage.Left = Label1(2).Left
    TargetObject.Width = Label1(0).Width
    txtFriendlyName.Width = Label1(1).Width
    txtMessage.Width = Label1(2).Width * 2 - 90
    i = Label1(0).Height + OutputWindow.Height
    TargetObject.Top = i
    txtFriendlyName.Top = i
    txtMessage.Top = i
    
    i = i + TargetObject.Height
    
    txtNativeLanguage.Top = i
    txtDistributions.Top = i
    txtSubscriptions.Top = i
    
    txtNativeLanguage.Width = Int(Width / 4)
    txtDistributions.Width = txtNativeLanguage.Width
    txtSubscriptions.Width = txtNativeLanguage.Width
    
    txtDistributions.Left = Label1(0).Left
    txtSubscriptions.Left = Label1(1).Left
    txtNativeLanguage.Left = Label1(2).Left
    CmdXMIT.Left = txtNativeLanguage.Left + txtNativeLanguage.Width + 20
    CmdXMIT.Top = txtNativeLanguage.Top
    
    cmdSelectResource.Top = CmdXMIT.Top
    cmdSelectResource.Left = CmdXMIT.Left + CmdXMIT.Width + 20
    

End Sub

Private Sub form_WriteProperties(PropBag As PropertyBag)
    Debug.Print Caption & " " & "WriteProperties"
    Call PropBag.WriteProperty("BackColor", m_BackColor, m_def_BackColor)
    Call PropBag.WriteProperty("Enabled", m_Enabled, m_def_Enabled)
    Call PropBag.WriteProperty("BackStyle", m_BackStyle, m_def_BackStyle)
    Call PropBag.WriteProperty("BorderStyle", m_BorderStyle, m_def_BorderStyle)
    'Call PropBag.WriteProperty("Image", m_Image, Nothing)
End Sub


Private Sub form_InitProperties()
    Debug.Print Caption & " " & "InitProperties"
    m_BackColor = m_def_BackColor
    m_Enabled = m_def_Enabled
    m_BackStyle = m_def_BackStyle
    m_BorderStyle = m_def_BorderStyle
   ' Set m_Image = LoadPicture("")
End Sub

Private Sub CancelButton_Click()
    Unload Me
End Sub

Private Sub cmdCancel_Click()
    CancelButton_Click
End Sub

Private Sub Form_Unload(Cancel As Integer)
    'Set TheConnection = Nothing
End Sub

Public Sub EnumAllLanguages()
  ' Dim i As Long, TypeArray As Variant
   ' txtNativeLanguage.Clear
    'TypeArray = TheQueryObject.Languages
  '  For i = 0 To UBound(TypeArray)
   '         txtNativeLanguage.AddItem TypeArray(i)
    'Next i
End Sub

Private Sub TheBotConnection_AnswerReady(ByVal BSTR As String)
  Dim d As String
    'Bstr = Trim(Replace(Bstr, "sample", ""))
   ' Bstr = Replace(Bstr, "Sample", "")
    BSTR = Trim(Replace(BSTR, "  ", " "))
    
        BSTR = Replace(BSTR, "''", "'")
        BSTR = Replace(BSTR, Chr(34), " ")
        BSTR = Replace(BSTR, ",,", ",")
        BSTR = Trim(Replace(BSTR, ", ,", ","))
        BSTR = Trim(Replace(BSTR, ",]", "]"))
        BSTR = Trim(Replace(BSTR, "|", " "))
    While Right(BSTR, 1) = ","
        BSTR = Trim(Left(BSTR, Len(BSTR) - 1))
       Wend
     '      SendToOutputWindow (BSTR)
    d = CStr(PrologObject.Execute("(check_cleanup(" & LCase(BSTR) & ", X),post(X))"))
    If Len(d) > 5 Then BSTR = d
    BSTR = Trim(BSTR) & vbCrLf
    SendToOutputWindow (BSTR)
    If InStr(BSTR, "please_exec") Then
        BSTR = Trim(BSTR)
        If InStr(BSTR, "[") Then BSTR = Left(BSTR, InStr(BSTR, "[") - 1)
        ''Replace(Replace(BStr, vbLf, " "), vbCr, " "))
        
        SendToOutputWindow (BSTR)
        d = CStr(PrologObject.Execute("(notice_vbscript(" & LCase(BSTR) & ", X),post(X))"))
        d = Replace(d, "'", "")
        SendToOutputWindow (d)
       theWorldWalker.InlineExec d
    End If
    
End Sub

