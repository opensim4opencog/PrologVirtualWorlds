VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.1#0"; "RICHTX32.OCX"
Begin VB.Form frmChannel 
   Caption         =   "Form1"
   ClientHeight    =   3255
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4635
   Icon            =   "channel.frx":0000
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   3255
   ScaleWidth      =   4635
   Begin VB.ListBox lstNames 
      Height          =   2700
      IntegralHeight  =   0   'False
      Left            =   3360
      Sorted          =   -1  'True
      TabIndex        =   2
      Top             =   0
      Width           =   1215
   End
   Begin VB.TextBox txtSend 
      Height          =   285
      Left            =   0
      TabIndex        =   1
      Top             =   2850
      Width           =   4575
   End
   Begin RichTextLib.RichTextBox txtText 
      Height          =   2655
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   3255
      _ExtentX        =   5741
      _ExtentY        =   4683
      _Version        =   327681
      Enabled         =   -1  'True
      ScrollBars      =   2
      TextRTF         =   $"channel.frx":0CCA
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Verdana"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
End
Attribute VB_Name = "frmChannel"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Resize()
    On Error Resume Next
    Dim i As Integer
    For i = 1 To 10
        If LCase(Me.Caption) = ChannelName(i) Then
            Channel(i).txtText.Move txtText.Left, txtText.Top, Channel(i).Width - 1500, Channel(i).Height - txtSend.Height - 500
            Channel(i).lstNames.Move txtText.Width, txtText.Left, 1400, Channel(i).Height - txtSend.Height - 505
            Channel(i).txtSend.Move txtText.Left, Channel(i).Height - 800, Channel(i).Width - 150, 350
        End If
    Next i
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Dim i As Integer
    
    mdiMain.tcp.SendData "PART " & Me.Caption & vbCrLf
    For i = 1 To 10
        If LCase(ChannelName(i)) = LCase(Me.Caption) Then
            ChannelName(i) = ""
        End If
    Next i
End Sub

Private Sub txtSend_KeyPress(KeyAscii As Integer)
    If KeyAscii = 13 Then
        mdiMain.tcp.SendData "PRIVMSG " & Me.Caption & " :" & txtSend & vbCrLf
        txtText.SelText = "<"
        txtText.SelBold = True
        txtText.SelColor = vbBlue
        txtText.SelText = NickName
        txtText.SelColor = vbBlack
        txtText.SelBold = False
        txtText.SelText = "> "
        txtText.SelText = txtSend & vbCrLf
        txtSend = ""
    End If
End Sub

Private Sub txtText_Change()
    txtText.SelStart = Len(txtText.Text)
End Sub
