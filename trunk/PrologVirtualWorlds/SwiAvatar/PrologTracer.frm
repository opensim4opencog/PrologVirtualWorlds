VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "richtx32.ocx"
Begin VB.Form fPrologTracer 
   AutoRedraw      =   -1  'True
   Caption         =   "Command Shell Tracer"
   ClientHeight    =   6990
   ClientLeft      =   1290
   ClientTop       =   5040
   ClientWidth     =   7395
   BeginProperty Font 
      Name            =   "Verdana"
      Size            =   9
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "PrologTracer.frx":0000
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   6990
   ScaleWidth      =   7395
   ShowInTaskbar   =   0   'False
   Visible         =   0   'False
   Begin RichTextLib.RichTextBox OutputWindow 
      Height          =   4785
      Left            =   180
      TabIndex        =   1
      Top             =   180
      Width           =   6990
      _ExtentX        =   12330
      _ExtentY        =   8440
      _Version        =   393217
      ScrollBars      =   3
      TextRTF         =   $"PrologTracer.frx":030A
   End
   Begin VB.ComboBox commandinput 
      Height          =   330
      Left            =   810
      TabIndex        =   0
      Top             =   5175
      Width           =   4470
   End
End
Attribute VB_Name = "fPrologTracer"
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

Public WithEvents theAvatarMorphism As AvatarMorphism
Attribute theAvatarMorphism.VB_VarHelpID = -1

Public LanguagesSpoken As VWSYSTEMLib.ipropertylist
Public LanguagesHeard As VWSYSTEMLib.ipropertylist

Private Sub commandinput_Change()
'If commandinput.Tag <> 13 Then Exit Sub
'    fPrologTracer.SendToOutputWindow (theSWIPrologShell.Execute(commandinput.Text))
'    commandinput.SelStart = 0
'    commandinput.SelLength = 100
End Sub



Private Sub commandinput_KeyPress(KeyAscii As Integer)
commandinput.Tag = KeyAscii
    If KeyAscii <> 13 Then Exit Sub
    SendToOutputWindow (theSWIPrologShell.Execute(commandinput.Text))
    commandinput.SelStart = 0
    commandinput.SelLength = 100
End Sub

Private Sub Form_Load()
    OutputWindow.LoadFile PVWLibraryPath & "\MOTD.Prolog", rtfText
    theSWIPrologShell.Consult PVWLibraryPath & "\proquery.prolog"
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
    If Len(t$) > 3 Then tr$ = theSWIPrologShell.Execute(t$)

    TextRight = tr$
'    Stop
    SendToOutputWindow (TextRight)
End Sub

Public Sub SendToOutputWindow(ByVal TextRight)
    On Error Resume Next
    TextRight = Trim(Replace(Replace(TextRight, vbLf, " "), vbCr, " ")) & vbCrLf
    OutputWindow.Text = OutputWindow.Text & vbCrLf
    OutputWindow.Text = Replace(OutputWindow.Text, vbCrLf & vbCrLf, vbCrLf)
    OutputWindow.Text = Replace(OutputWindow.Text, vbCrLf & vbCrLf, vbCrLf)
    OutputWindow.Text = OutputWindow.Text & TextRight
    
    OutputWindow.SelStart = Len(OutputWindow.Text)
    
    'VB.Clipboard.SetText Trim(TextRight) & vbCrLf, ClipBoardConstants.vbCFText
    'OutputWindow.Paste
End Sub


Private Sub OutputWindow_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button = 2 Then Me.PopupMenu theMainForm.mnuEdit

End Sub

Private Sub OutputWindow_SelChange()
On Error Resume Next
   ' theMainForm.tbToolBar.Buttons("Bold").Value = IIf(OutputWindow.SelBold, tbrPressed, tbrUnpressed)
    'theMainForm.tbToolBar.Buttons("Italic").Value = IIf(OutputWindow.SelItalic, tbrPressed, tbrUnpressed)
    'theMainForm.tbToolBar.Buttons("Underline").Value = IIf(OutputWindow.SelUnderline, tbrPressed, tbrUnpressed)
    'theMainForm.tbToolBar.Buttons("Align Left").Value = IIf(OutputWindow.SelAlignment = rtfLeft, tbrPressed, tbrUnpressed)
    'theMainForm.tbToolBar.Buttons("Center").Value = IIf(OutputWindow.SelAlignment = rtfCenter, tbrPressed, tbrUnpressed)
    'theMainForm.tbToolBar.Buttons("Align Right").Value = IIf(OutputWindow.SelAlignment = rtfRight, tbrPressed, tbrUnpressed)
End Sub

Private Sub Form_Resize()
    On Error Resume Next

    OutputWindow.Top = 0
    OutputWindow.Left = 0
    OutputWindow.Height = Height - commandinput.Height - 500
    OutputWindow.Width = Width - 100
    commandinput.Top = OutputWindow.Height
    commandinput.Left = 0
    commandinput.Width = OutputWindow.Width

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
Cancel = 0
    Me.WindowState = vbMinimized
    'If Me.Enabled = True Then Cancel = 1
    
End Sub



Private Sub OutputWindow_StatusChange(ByVal lSelTyp As Long, ByVal lSelBegLine As Long, ByVal lSelBegChar As Long, ByVal lSelEndLine As Long, ByVal lSelEndChar As Long, ByVal lSelLen As Long, ByVal lLine As Long, ByVal lChar As Long, ByVal lcOut As Long, ByVal lInsertMode As Long, ByVal bModified As Long, ByVal bReadOnly As Long, ByVal lLines As Long, ByVal lChars As Long, ByVal bForce As Long)
'Stop
If lSelEndChar = 0 Then Exit Sub
'MsgBox lSelEndChar
End Sub

Private Sub theAvatarMorphism_AnswerReady(ByVal textBSTR As String)
  Dim d As String
    'textBSTR = Trim(Replace(textBSTR, "sample", ""))
   ' textBSTR = Replace(textBSTR, "Sample", "")
    textBSTR = Trim(Replace(textBSTR, "  ", " "))
    
        textBSTR = Replace(textBSTR, "''", "'")
        textBSTR = Replace(textBSTR, Chr(34), " ")
        textBSTR = Replace(textBSTR, ",,", ",")
        textBSTR = Trim(Replace(textBSTR, ", ,", ","))
        textBSTR = Trim(Replace(textBSTR, ",]", "]"))
        textBSTR = Trim(Replace(textBSTR, "|", " "))
    While Right(textBSTR, 1) = ","
        textBSTR = Trim(Left(textBSTR, Len(textBSTR) - 1))
       Wend
     '      SendToOutputWindow (textBSTR)
    d = CStr(theSWIPrologShell.Execute("(check_cleanup(" & LCase(textBSTR) & ", X),post(X))"))
    If Len(d) > 5 Then textBSTR = d
    textBSTR = Trim(textBSTR) & vbCrLf
    SendToOutputWindow (textBSTR)
    If InStr(textBSTR, "please_exec") Then
        textBSTR = Trim(textBSTR)
        If InStr(textBSTR, "[") Then textBSTR = Left(textBSTR, InStr(textBSTR, "[") - 1)
        ''Replace(Replace(textBSTR, vbLf, " "), vbCr, " "))
        
        SendToOutputWindow (textBSTR)
        d = CStr(theSWIPrologShell.Execute("(notice_vbscript(" & LCase(textBSTR) & ", X),post(X))"))
        d = Replace(d, "'", "")
        SendToOutputWindow (d)
    End If
    
End Sub

