VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "richtx32.ocx"
Begin VB.Form fComClassBrowser 
   Caption         =   "ComClassBrowser"
   ClientHeight    =   6000
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9255
   BeginProperty Font 
      Name            =   "Verdana"
      Size            =   9.75
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "ComClassBrowser.frx":0000
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   6000
   ScaleWidth      =   9255
   Visible         =   0   'False
   Begin VB.CommandButton SelectList 
      Caption         =   "Load"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   345
      Left            =   5355
      TabIndex        =   4
      Top             =   45
      Width           =   915
   End
   Begin RichTextLib.RichTextBox OutputWindow 
      Height          =   2115
      Left            =   60
      TabIndex        =   3
      Top             =   2160
      Width           =   6255
      _ExtentX        =   11033
      _ExtentY        =   3731
      _Version        =   393217
      Enabled         =   -1  'True
      ScrollBars      =   3
      TextRTF         =   $"ComClassBrowser.frx":19BA
   End
   Begin VB.ComboBox Text1 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Left            =   45
      TabIndex        =   2
      Text            =   "<load DLL>"
      Top             =   45
      Width           =   5250
   End
   Begin VB.ListBox List2 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1620
      Left            =   3135
      TabIndex        =   1
      Top             =   465
      Width           =   3180
   End
   Begin VB.ListBox List1 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1620
      Left            =   60
      TabIndex        =   0
      Top             =   480
      Width           =   3000
   End
End
Attribute VB_Name = "fComClassBrowser"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

'Given a VB form with two ListBox controls, a TextBox, a Label,
'and a Command Button, load a type library based on Text1,
'show types in List1 and members in List2.  The PrototypeMember
'function is defined in the linked topic.

Private m_TLInf As TypeLibInfo

Private Sub Form_Load()
  Me.Caption = "  Prolog Object Browser - " & Text1
  Set m_TLInf = New TypeLibInfo
  m_TLInf.AppObjString = "<Unqualified>"
End Sub

Private Sub SelectList_Click()
Text1 = theMainForm.CommonDialog("Open", False, "ObjectClass Files (*.dll;*.ocx;*.tlb)|*.dll;*.ocx;*.tlb", "open")
  theCOMMorphism.add_com_dll Text1
  On Error Resume Next
  m_TLInf.ContainingFile = Text1
  Me.Caption = "  Prolog Object Browser - " & Text1
  If Err Then Beep: Exit Sub
  List2.Clear
  With List1
    .Clear
    m_TLInf.GetTypesDirect .hWnd
    If .ListCount Then .ListIndex = 0
  End With
End Sub


Private Sub Form_Resize()
    On Error Resume Next
    Dim w
    w = Int(Me.ScaleWidth / 2)
    Text1.Left = 25
    List1.Left = 25
    OutputWindow.Left = 20
    List2.Top = List1.Top
    
    Text1.Width = w - 25: SelectList.Left = w + 25
    List1.Width = w - 25: List2.Left = w + 25
    List2.Width = w - 25
    
    OutputWindow.Width = Me.ScaleWidth - 15
    OutputWindow.Height = Me.ScaleHeight - 2200
End Sub


Private Sub list1_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
If Button = 2 Then Me.PopupMenu theMainForm.mnuComBrowserObj
End Sub
Private Sub list2_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
If Button = 2 Then Me.PopupMenu theMainForm.mnuComBrowserCopyProp
End Sub

Private Sub List1_Click()
  On Error Resume Next
  With List1
    List2.Clear
    'Retrieve the SearchData from the ItemData property
    m_TLInf.GetMembersDirect .ItemData(.ListIndex), List2.hWnd
  End With
  List2.ListIndex = 0
  List2_Click
End Sub
Private Sub List2_Click()
Dim InvKinds As tli.InvokeKinds
With List2
    InvKinds = .ItemData(.ListIndex)
    OutputWindow.Text = theCOMMorphism.PrototypeMember(m_TLInf, List1.ItemData(List1.ListIndex), InvKinds, , .[_Default])
End With
End Sub

Private Sub OutputWindow_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
If Button = 2 Then Me.PopupMenu theMainForm.mnuEdit
End Sub

Private Sub Text1_Click()
  On Error Resume Next
  m_TLInf.ContainingFile = Text1
  Me.Caption = "  Prolog Object Browser - " & Text1
  If Err Then Beep: Exit Sub
  List2.Clear
  With List1
    .Clear
    m_TLInf.GetTypesDirect .hWnd
    If .ListCount Then .ListIndex = 0
  End With
End Sub

Private Sub Text1_KeyPress(KeyAscii As Integer)
If KeyAscii <> 13 Then Exit Sub
    theCOMMorphism.add_com_dll Text1.Text
End Sub
