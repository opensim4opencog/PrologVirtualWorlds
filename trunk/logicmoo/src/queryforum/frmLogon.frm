VERSION 5.00
Begin VB.Form frmLogon 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Login"
   ClientHeight    =   4245
   ClientLeft      =   4065
   ClientTop       =   4035
   ClientWidth     =   6855
   Icon            =   "frmLogon.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4245
   ScaleWidth      =   6855
   Begin VB.TextBox txtUserName 
      Height          =   285
      Left            =   2055
      TabIndex        =   5
      Top             =   2310
      Width           =   2325
   End
   Begin VB.TextBox txtPassword 
      Height          =   285
      IMEMode         =   3  'DISABLE
      Left            =   2040
      PasswordChar    =   "*"
      TabIndex        =   4
      Top             =   2685
      Width           =   2325
   End
   Begin VB.CommandButton cmdCancel1 
      Cancel          =   -1  'True
      Caption         =   "Elsewhere"
      Height          =   360
      Left            =   5235
      TabIndex        =   3
      Tag             =   "1070"
      Top             =   3480
      Width           =   1140
   End
   Begin VB.CommandButton cmdOK1 
      Caption         =   "Microsoft"
      Default         =   -1  'True
      Height          =   360
      Left            =   4035
      TabIndex        =   2
      Tag             =   "1069"
      Top             =   3480
      Width           =   1140
   End
   Begin VB.Label lblCompanyProduct1 
      AutoSize        =   -1  'True
      Caption         =   "Douglas R. Miles"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   240
      Index           =   1
      Left            =   4860
      TabIndex        =   15
      Tag             =   "1063"
      Top             =   825
      Width           =   1785
   End
   Begin VB.Label lblCompanyProduct1 
      AutoSize        =   -1  'True
      Caption         =   "Douglas R. Miles"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   240
      Index           =   0
      Left            =   465
      TabIndex        =   14
      Tag             =   "1063"
      Top             =   6060
      Width           =   1785
   End
   Begin VB.Label lblPlatform1 
      Alignment       =   1  'Right Justify
      AutoSize        =   -1  'True
      Caption         =   "Windows 2000 Only (Whistler Ok)"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   13.5
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   360
      Index           =   1
      Left            =   300
      TabIndex        =   13
      Tag             =   "1062"
      Top             =   5235
      Width           =   4680
   End
   Begin VB.Label lblPlatform1 
      Alignment       =   1  'Right Justify
      AutoSize        =   -1  'True
      Caption         =   "Windows 2000 Only (Whistler Ok)"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   13.5
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   360
      Index           =   0
      Left            =   1200
      TabIndex        =   12
      Tag             =   "1062"
      Top             =   1470
      Width           =   4680
   End
   Begin VB.Label lblLicenseTo1 
      Alignment       =   1  'Right Justify
      Caption         =   "Licensed To Corporate LAN"
      Height          =   285
      Index           =   1
      Left            =   255
      TabIndex        =   11
      Tag             =   "1065"
      Top             =   4830
      Width           =   2070
   End
   Begin VB.Label lblProductName1 
      AutoSize        =   -1  'True
      Caption         =   "Query Forum"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   29.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   675
      Index           =   1
      Left            =   2130
      TabIndex        =   10
      Tag             =   "1064"
      Top             =   5865
      Width           =   3675
   End
   Begin VB.Label Label1 
      Caption         =   "Query Forum"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   29.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   705
      Left            =   2715
      TabIndex        =   9
      Top             =   60
      Width           =   3945
   End
   Begin VB.Label lblWarning1 
      Caption         =   "      Not Distributable..                Where are you today?"
      Height          =   195
      Left            =   75
      TabIndex        =   8
      Tag             =   "1060"
      Top             =   3600
      Width           =   6855
   End
   Begin VB.Label lblLabels 
      Caption         =   "&User Name:"
      Height          =   255
      Index           =   0
      Left            =   960
      TabIndex        =   7
      Tag             =   "1067"
      Top             =   2355
      Width           =   1080
   End
   Begin VB.Label lblLabels 
      Caption         =   "&Password:"
      Height          =   255
      Index           =   1
      Left            =   960
      TabIndex        =   6
      Tag             =   "1068"
      Top             =   2730
      Width           =   1080
   End
   Begin VB.Label lblVersion1 
      Alignment       =   1  'Right Justify
      AutoSize        =   -1  'True
      Caption         =   "Version 0.6"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   300
      Left            =   4575
      TabIndex        =   1
      Tag             =   "1061"
      Top             =   2745
      Width           =   1380
   End
   Begin VB.Label lblCompany1 
      Caption         =   "(c) Microsoft Research"
      Height          =   255
      Left            =   4575
      TabIndex        =   0
      Tag             =   "1059"
      Top             =   2370
      Width           =   1920
   End
End
Attribute VB_Name = "frmLogon"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Function GetUserName Lib "advapi32.dll" Alias "GetUserNameA" (ByVal lpbuffer As String, nSize As Long) As Long


Public OK As Boolean

Private Sub cmdOK1_Click()
    'ToDo: create test for correct password
    'check for correct password
    If txtPassword.Text = "" Then
        OK = True

    Else
        MsgBox "Invalid Password, try again!", , "Login"
        txtPassword.SetFocus
        txtPassword.SelStart = 0
        txtPassword.SelLength = Len(txtPassword.Text)
    End If
    
   ' Shell "C:\Program Files\Microsoft Virtual Worlds\vws.exe"
    
    'Main1
    Unload Me
End Sub

Private Sub Form_Load()
    'Dim lSize As Long
    'LoadResStrings Me
    
    sBuffer = Space$(255)
    lSize = Len(sBuffer)
    Call GetUserName(sBuffer, lSize)
    If lSize > 0 Then
        txtUserName.Text = Left$(sBuffer, lSize)
    Else
        txtUserName.Text = vbNullString
    End If
 '   Unload Me
End Sub



Private Sub cmdCancel1_Click()
    OK = False
    Unload Me
End Sub





Private Sub fraMainFrame_DragDrop(Source As Control, X As Single, Y As Single)

End Sub

