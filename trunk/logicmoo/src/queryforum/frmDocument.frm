VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "richtx32.ocx"
Begin VB.Form fDocument 
   Caption         =   "frmDocument"
   ClientHeight    =   4995
   ClientLeft      =   4110
   ClientTop       =   4635
   ClientWidth     =   7095
   BeginProperty Font 
      Name            =   "Verdana"
      Size            =   9.75
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "frmDocument.frx":0000
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   4995
   ScaleWidth      =   7095
   Begin RichTextLib.RichTextBox OutputWindow 
      Height          =   4425
      Left            =   180
      TabIndex        =   0
      Top             =   135
      Width           =   6585
      _ExtentX        =   11615
      _ExtentY        =   7805
      _Version        =   393217
      Enabled         =   -1  'True
      ScrollBars      =   3
      TextRTF         =   $"frmDocument.frx":000C
   End
End
Attribute VB_Name = "fDocument"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Public IsModified As Boolean

Private Sub OutputWindow_KeyUp(KeyCode As Integer, Shift As Integer)
IsModified = True
End Sub

Private Sub OutputWindow_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
If Button = 2 Then Me.PopupMenu theMainForm.mnuEdit
End Sub

Private Sub OutputWindow_SelChange()
On Error Resume Next
IsModified = True
    theMainForm.tbToolBar.Buttons("Bold").Value = IIf(OutputWindow.SelBold, tbrPressed, tbrUnpressed)
    theMainForm.tbToolBar.Buttons("Italic").Value = IIf(OutputWindow.SelItalic, tbrPressed, tbrUnpressed)
    theMainForm.tbToolBar.Buttons("Underline").Value = IIf(OutputWindow.SelUnderline, tbrPressed, tbrUnpressed)
    theMainForm.tbToolBar.Buttons("Align Left").Value = IIf(OutputWindow.SelAlignment = rtfLeft, tbrPressed, tbrUnpressed)
    theMainForm.tbToolBar.Buttons("Center").Value = IIf(OutputWindow.SelAlignment = rtfCenter, tbrPressed, tbrUnpressed)
    theMainForm.tbToolBar.Buttons("Align Right").Value = IIf(OutputWindow.SelAlignment = rtfRight, tbrPressed, tbrUnpressed)
End Sub



Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
MsgBox KeyCode & Shift
End Sub

Private Sub Form_Load()
IsModified = False
Dim OutputWindowOptions(18) As Long
    Dim VARARRY
  'outputwindow.NewFile
  Dim i
  'VARARRY = outputwindow.Options
  'For i = 0 To 18: OutputWindowOptions(i) = VARARRY(i): Next
    
   ' Stop
   ' OutputWindowOptions(0) = vbTrue
   ' OutputWindowOptions(6) = 0
   ' OutputWindowOptions(6) = 0
   ' OutputWindowOptions(1) = Asc("b")
   ' OutputWindowOptions(8) = 0

 ' OutputWindowOptions(3) = 211 '  int nOpenCode;  // file open format 211-Auto 212-Win 213-Dos 214-Mac 215-Unix

'  OutputWindowOptions(7) = 0 ' BOOL    bReadOnly;  // open file as read-only
 ' OutputWindowOptions(10) = 6 ' int nTab;   // tab size in spaces
'  OutputWindowOptions(12) = 0
'  OutputWindowOptions(13) = 0
 ' OutputWindowOptions(14) = 0
 ' OutputWindowOptions(15) = 0
'  OutputWindowOptions(16) = 0
'  OutputWindow.Options = OutputWindowOptions
    Form_Resize
    
End Sub


Private Sub Form_Resize()
    On Error Resume Next
    OutputWindow.Move 10, 10, Me.ScaleWidth - 20, Me.ScaleHeight - 20
    'OutputWindow.r = OutputWindow.Width - 400
End Sub

Private Sub Form_Terminate()
    ConfirmSave
    OutputWindow.Visible = False
    Unload Me
End Sub

Sub ConfirmSave()
    'If outputwindow.IsModified Then AskSave
    If IsModified Then AskSave
End Sub

Sub AskSave()
    Me.SetFocus
   If vbYes = MsgBox(Me.Caption & " has not been saved since last modified." & vbCrLf & "Would you like to save it now to " & Me.Caption & "?", vbYesNo, "Confirm Changes") Then
        DoSave
    End If
End Sub

Sub DoSave()
        OutputWindow.SaveFile Me.Caption, rtfText
    IsModified = False
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ConfirmSave
    OutputWindow.Visible = False
    Unload Me
End Sub

Private Sub OutputWindow_Command(ByVal nCmd As Long)
MsgBox nCmd
End Sub

Private Sub OutputWindow_Message(ByVal nIdMessage As Long, ByVal lpszParameter As String)
MsgBox nIdMessage & lpszParameter
End Sub

Private Sub OutputWindow_StatusChange(ByVal lSelTyp As Long, ByVal lSelBegLine As Long, ByVal lSelBegChar As Long, ByVal lSelEndLine As Long, ByVal lSelEndChar As Long, ByVal lSelLen As Long, ByVal lLine As Long, ByVal lChar As Long, ByVal lcOut As Long, ByVal lInsertMode As Long, ByVal bModified As Long, ByVal bReadOnly As Long, ByVal lLines As Long, ByVal lChars As Long, ByVal bForce As Long)
'MsgBox theMorphism.object_array_pl_string(lSelTyp, lSelBegLine, lSelBegChar, lSelEndLine, lSelEndChar, lSelLen, lLine, lChar, lcOut, lInsertMode, bModified, bReadOnly, lLines, lChars, bForce)
End Sub
