VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Begin VB.Form frmDocument 
   Caption         =   "Telnet Log"
   ClientHeight    =   3195
   ClientLeft      =   11220
   ClientTop       =   5895
   ClientWidth     =   4680
   Icon            =   "frmDocument.frx":0000
   LinkMode        =   1  'Source
   LinkTopic       =   "IRC"
   MDIChild        =   -1  'True
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   Begin RichTextLib.RichTextBox TheWindow 
      Height          =   2085
      Left            =   1815
      TabIndex        =   0
      Top             =   990
      Width           =   1920
      _ExtentX        =   3387
      _ExtentY        =   3678
      _Version        =   393217
      TextRTF         =   $"frmDocument.frx":0442
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
End
Attribute VB_Name = "frmDocument"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()
    Form_Resize
End Sub


Private Sub Form_Resize()
    On Error Resume Next
    theWindow.Move 5, 10, Me.ScaleWidth - 200, Me.ScaleHeight - 20
    theWindow.Width = Me.Width - 90
End Sub

Private Sub Form_Unload(Cancel As Integer)
'Me.Hide
'If fMainForm.ClosingApp.Value = 0 Then Cancel = vbCancel
End Sub

Private Sub thewindow_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
   Me.Show
   Me.theWindow.SetFocus
   If Button = 2 Then   ' Check if right mouse button
                        ' was clicked.
      PopupMenu fMainForm.mnuEdit   ' Display the File menu as a
                        ' pop-up menu.
   End If
End Sub




