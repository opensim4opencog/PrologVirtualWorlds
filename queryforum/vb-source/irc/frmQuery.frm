VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Begin VB.Form Query 
   Caption         =   "Private Message Window"
   ClientHeight    =   2775
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5520
   Icon            =   "frmQuery.frx":0000
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   2775
   ScaleWidth      =   5520
   Begin RichTextLib.RichTextBox txtQuery 
      Height          =   2175
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   5415
      _ExtentX        =   9551
      _ExtentY        =   3836
      _Version        =   393217
      Enabled         =   -1  'True
      TextRTF         =   $"frmQuery.frx":1042
   End
   Begin VB.TextBox txtSend 
      Height          =   375
      Left            =   0
      TabIndex        =   0
      Top             =   2280
      Width           =   5415
   End
End
Attribute VB_Name = "Query"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public strNick
Private Sub Form_Resize()
    On Error Resume Next
        
    txtQuery.Move txtQuery.Left, txtQuery.Top, Me.Width - 150, Me.Height - 700
    txtSend.Move txtQuery.Left, txtQuery.Height - 10, Me.Width - 150, 300
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Dim i As Integer
    
    For i = 1 To 100
        If LCase(Me.Caption) = LCase(QueryName(i)) Then
            QueryName(i) = ""
        End If
    Next i
End Sub

Private Sub txtQuery_Change()
    txtQuery.SelStart = Len(txtQuery.Text)
End Sub

Private Sub txtSend_KeyPress(KeyAscii As Integer)
    If KeyAscii = 13 Then
        mdiMain.tcp.SendData "PRIVMSG " & Me.Caption & " :" & txtSend & vbCrLf
        txtQuery.SelText = "<"
        txtQuery.SelBold = True
        txtQuery.SelColor = vbBlue
        txtQuery.SelText = NickName
        txtQuery.SelColor = vbBlack
        txtQuery.SelBold = False
        txtQuery.SelText = "> "
        txtQuery.SelText = txtSend & vbCrLf
        txtSend = ""
    End If
End Sub
