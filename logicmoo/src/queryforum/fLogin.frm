VERSION 5.00
Begin VB.Form fLogin 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Login to Virtual Worlds"
   ClientHeight    =   2100
   ClientLeft      =   2835
   ClientTop       =   3480
   ClientWidth     =   4080
   Icon            =   "fLogin.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1240.749
   ScaleMode       =   0  'User
   ScaleWidth      =   3830.899
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.ComboBox txtWorldName 
      Height          =   315
      Left            =   1305
      TabIndex        =   7
      Text            =   "worlds.dev.aliensystems.com/sample"
      Top             =   900
      Width           =   2355
   End
   Begin VB.TextBox txtUserName 
      Height          =   345
      Left            =   1290
      TabIndex        =   1
      Top             =   135
      Width           =   2325
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   390
      Left            =   1125
      TabIndex        =   4
      Top             =   1440
      Width           =   1140
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   390
      Left            =   2475
      TabIndex        =   5
      Top             =   1440
      Width           =   1140
   End
   Begin VB.TextBox txtPassword 
      Height          =   345
      IMEMode         =   3  'DISABLE
      Left            =   1290
      PasswordChar    =   "*"
      TabIndex        =   3
      Top             =   525
      Width           =   2325
   End
   Begin VB.Label Label1 
      Caption         =   "World"
      Height          =   240
      Left            =   135
      TabIndex        =   6
      Top             =   900
      Width           =   1005
   End
   Begin VB.Label lblLabels 
      Caption         =   "&User Name:"
      Height          =   270
      Index           =   0
      Left            =   105
      TabIndex        =   0
      Top             =   150
      Width           =   1080
   End
   Begin VB.Label lblLabels 
      Caption         =   "&Password:"
      Height          =   270
      Index           =   1
      Left            =   105
      TabIndex        =   2
      Top             =   540
      Width           =   1080
   End
End
Attribute VB_Name = "fLogin"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdCancel_Click()
    LoginSucceeded = False
    Unload Me
End Sub

Private Sub cmdOK_Click()
    LoginSucceeded = True
    HumanAvatarName = txtUserName.Text
    HumanAvatarPass = txtPassword.Text
    theWorldName = txtWorldName.Text
    Unload Me
End Sub

Private Sub Form_Load()
    LoginSucceeded = False
    txtUserName.Text = HumanAvatarName
    txtPassword.Text = HumanAvatarPass
    txtWorldName.Text = theWorldName
End Sub
