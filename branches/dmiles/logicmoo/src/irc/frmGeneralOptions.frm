VERSION 5.00
Begin VB.Form frmGeneralOptions 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Cabral General Options"
   ClientHeight    =   4395
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   3375
   Icon            =   "frmGeneralOptions.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MDIChild        =   -1  'True
   MinButton       =   0   'False
   ScaleHeight     =   4395
   ScaleWidth      =   3375
   ShowInTaskbar   =   0   'False
   Begin VB.ComboBox cmbNickname 
      Height          =   315
      Left            =   120
      TabIndex        =   13
      Text            =   "UnafX"
      Top             =   960
      Width           =   3135
   End
   Begin VB.CommandButton cmdAddNickname 
      Height          =   135
      Left            =   3120
      TabIndex        =   12
      Top             =   840
      Width           =   135
   End
   Begin VB.CommandButton cmdAddServer 
      Height          =   135
      Left            =   3120
      TabIndex        =   11
      Top             =   120
      Width           =   135
   End
   Begin VB.Frame frmWhat 
      Caption         =   "Options"
      Height          =   1215
      Left            =   120
      TabIndex        =   9
      Top             =   2640
      Width           =   3135
      Begin VB.Label lblWhat 
         Caption         =   "Thank you for using Cabral IRC Client"
         Height          =   855
         Left            =   120
         TabIndex        =   10
         Top             =   240
         Width           =   2895
      End
   End
   Begin VB.TextBox txtRealName 
      Height          =   285
      Left            =   120
      TabIndex        =   7
      Text            =   "Cabral IRC Client"
      Top             =   2160
      Width           =   3135
   End
   Begin VB.TextBox txtEmail 
      Height          =   285
      Left            =   120
      TabIndex        =   6
      Text            =   "cabral"
      Top             =   1560
      Width           =   3135
   End
   Begin VB.CommandButton cmdConnect 
      Caption         =   "Connect"
      Height          =   375
      Left            =   1200
      TabIndex        =   2
      Top             =   3960
      Width           =   975
   End
   Begin VB.CommandButton cmdCancel 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   2280
      TabIndex        =   1
      Top             =   3960
      Width           =   975
   End
   Begin VB.ComboBox cmbServers 
      Appearance      =   0  'Flat
      BackColor       =   &H00000000&
      ForeColor       =   &H0000FF00&
      Height          =   315
      Left            =   120
      TabIndex        =   0
      Text            =   "127.0.0.1"
      Top             =   240
      Width           =   3135
   End
   Begin VB.Label Label4 
      Caption         =   "Real Name/Phrase:"
      Height          =   255
      Left            =   120
      TabIndex        =   8
      Top             =   1920
      Width           =   3135
   End
   Begin VB.Label Label3 
      Caption         =   "Username:"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   1320
      Width           =   2415
   End
   Begin VB.Label Label2 
      Caption         =   "Nickname:"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   720
      Width           =   1935
   End
   Begin VB.Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Server:"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   0
      Width           =   2175
   End
End
Attribute VB_Name = "frmGeneralOptions"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub cmbNickname_GotFocus()
    lblWhat.Caption = "Type in the nickname you want to use while chatting"
End Sub

Private Sub cmbNickname_LostFocus()
    lblWhat.Caption = ""
End Sub

Private Sub cmdCancel_Click()
    Unload Me
End Sub

Private Sub cmdConnect_Click()
    email = txtEmail
    RealName = txtRealName
    NickName = cmbNickname.Text
    With mdiMain.tcp
        If .State <> 0 Then
            .Close
        End If
        .Connect cmbServers.Text, 6667
    End With
    Unload Me
End Sub

Private Sub Form_Load()
    cmbServers.AddItem "irc.chat.org"
    cmbServers.AddItem "irc.snowhill.com"
    cmbServers.AddItem "irc.dal.net"
    cmbServers.AddItem "127.0.0.1"
End Sub



Private Sub txtEmail_GotFocus()
        lblWhat.Caption = "Enter first part of your email address - which is known as your username (e.g. username@email.com)"
End Sub

Private Sub txtEmail_LostFocus()
        lblWhat.Caption = ""

End Sub

Private Sub txtRealName_GotFocus()
        lblWhat.Caption = "Enter your real name, a quote, or a little message"
End Sub

Private Sub txtRealName_LostFocus()
        lblWhat.Caption = ""
End Sub
