VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "richtx32.ocx"
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Begin VB.Form NLProxy 
   Caption         =   "NLProxy"
   ClientHeight    =   3675
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4470
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   3675
   ScaleWidth      =   4470
   Visible         =   0   'False
   Begin MSWinsockLib.Winsock Winsock1 
      Left            =   4560
      Top             =   120
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      RemoteHost      =   "localhost"
      RemotePort      =   2222
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Left            =   0
      TabIndex        =   8
      Text            =   "Text1"
      Top             =   0
      Width           =   4455
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Prolog"
      Height          =   495
      Left            =   0
      TabIndex        =   7
      Top             =   360
      Width           =   1335
   End
   Begin VB.CommandButton Command2 
      Caption         =   "NLParser"
      Height          =   495
      Left            =   1560
      TabIndex        =   6
      Top             =   360
      Width           =   1335
   End
   Begin VB.CommandButton cmdDecode 
      Caption         =   "Decode"
      Height          =   495
      Left            =   0
      TabIndex        =   3
      Top             =   2040
      Width           =   1335
   End
   Begin VB.CommandButton cmdRecode 
      Caption         =   "Recode"
      Height          =   495
      Left            =   1560
      TabIndex        =   2
      Top             =   2040
      Width           =   1335
   End
   Begin VB.CommandButton cmdRegen 
      Caption         =   "Re-gen"
      Height          =   495
      Left            =   3120
      TabIndex        =   1
      Top             =   360
      Width           =   1335
   End
   Begin VB.CommandButton cmdReConsult 
      Caption         =   "Reconsult"
      Height          =   495
      Left            =   3120
      TabIndex        =   0
      Top             =   2040
      Width           =   1215
   End
   Begin RichTextLib.RichTextBox BinParse 
      Height          =   975
      Left            =   0
      TabIndex        =   4
      Top             =   2640
      Width           =   4455
      _ExtentX        =   7858
      _ExtentY        =   1720
      _Version        =   393217
      Enabled         =   -1  'True
      TextRTF         =   $"NLProxy.frx":0000
   End
   Begin RichTextLib.RichTextBox NLPQue 
      Height          =   975
      Left            =   0
      TabIndex        =   5
      Top             =   960
      Width           =   4455
      _ExtentX        =   7858
      _ExtentY        =   1720
      _Version        =   393217
      Enabled         =   -1  'True
      ScrollBars      =   3
      TextRTF         =   $"NLProxy.frx":0085
   End
End
Attribute VB_Name = "NLProxy"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub Command1_Click()
        Winsock1.SendData Text1.Text
End Sub

Public Sub Command2_Click()
        Winsock1.SendData Text1.Text
End Sub

Private Sub Command3_Click()
      Text1.LinkMode = 0
      ' Set the application name and topic name.
      Text1.LinkTopic = Target.Text
      Text1.LinkItem = Item   ' Set LinkItem.
'      Text1.LinkMode = 1   ' Set LinkMode to Automatic.
'      Text1.LinkPoke
End Sub


Private Sub Winsock1_DataArrival(ByVal bytesTotal As Long)
   Dim strData As String
   Winsock1.GetData strData, vbString
   NLPQue.Text = strData
End Sub
