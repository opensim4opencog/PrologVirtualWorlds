VERSION 5.00
Begin VB.Form ConnectForm 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Connect To..."
   ClientHeight    =   2130
   ClientLeft      =   12390
   ClientTop       =   7395
   ClientWidth     =   3975
   Icon            =   "ConnectForm.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2130
   ScaleWidth      =   3975
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton Command1 
      Cancel          =   -1  'True
      Caption         =   "Ping"
      Enabled         =   0   'False
      Height          =   330
      Left            =   1320
      TabIndex        =   7
      Top             =   1680
      Width           =   960
   End
   Begin VB.CommandButton cmdCancel 
      Caption         =   "Cancel"
      Height          =   330
      Left            =   2880
      TabIndex        =   5
      Top             =   1680
      Width           =   960
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "&OK"
      Height          =   330
      Left            =   120
      TabIndex        =   4
      Top             =   1680
      Width           =   1065
   End
   Begin VB.Frame fraStep3 
      Caption         =   "Connection Values"
      Height          =   1455
      Index           =   0
      Left            =   120
      TabIndex        =   6
      Top             =   120
      Width           =   3735
      Begin VB.ComboBox ServerLoc 
         Height          =   315
         ItemData        =   "ConnectForm.frx":0442
         Left            =   1080
         List            =   "ConnectForm.frx":0449
         TabIndex        =   0
         Text            =   "192.168.200.145"
         Top             =   360
         Width           =   1935
      End
      Begin VB.TextBox Port 
         Height          =   300
         Left            =   1080
         TabIndex        =   2
         Text            =   "5000"
         Top             =   720
         Width           =   1215
      End
      Begin VB.Label lblStep3 
         AutoSize        =   -1  'True
         Caption         =   "Port:"
         Height          =   195
         Index           =   2
         Left            =   240
         TabIndex        =   1
         Top             =   720
         Width           =   330
      End
      Begin VB.Label lblStep3 
         AutoSize        =   -1  'True
         Caption         =   "&Server:"
         Height          =   255
         Index           =   6
         Left            =   240
         TabIndex        =   3
         Top             =   360
         Width           =   615
      End
   End
End
Attribute VB_Name = "ConnectForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdCancel_Click()
Unload Me

End Sub

Private Sub cmdOK_Click()
fMainForm.Server = Me.ServerLoc
fMainForm.Port = Me.Port

Unload Me

End Sub

