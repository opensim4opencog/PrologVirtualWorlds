VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form Options 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Options"
   ClientHeight    =   4980
   ClientLeft      =   7080
   ClientTop       =   1875
   ClientWidth     =   5625
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H00FFFFFF&
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4980
   ScaleWidth      =   5625
   ShowInTaskbar   =   0   'False
   Begin VB.PictureBox picIRC 
      BorderStyle     =   0  'None
      Height          =   4500
      Left            =   1995
      ScaleHeight     =   4500
      ScaleWidth      =   3585
      TabIndex        =   49
      Top             =   390
      Width           =   3585
      Begin VB.CheckBox chkHidePing 
         Appearance      =   0  'Flat
         Caption         =   " Hide Ping? Pong! "
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   90
         TabIndex        =   55
         Top             =   690
         Width           =   2700
      End
      Begin VB.CheckBox chkRejoin 
         Appearance      =   0  'Flat
         Caption         =   " Rejoin Channels when kicked "
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   90
         TabIndex        =   54
         Top             =   375
         Width           =   2700
      End
      Begin VB.CheckBox chkWhoIsQ 
         Appearance      =   0  'Flat
         Caption         =   " Do WhoIs in Queries"
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   90
         TabIndex        =   50
         Top             =   75
         Width           =   3405
      End
   End
   Begin VB.CheckBox chkOP 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00800000&
      Caption         =   " I-face/Windows..."
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   300
      Index           =   5
      Left            =   135
      TabIndex        =   46
      Top             =   2100
      Width           =   1680
   End
   Begin VB.CommandButton cmdCancel 
      Caption         =   "&Cancel"
      Height          =   360
      Left            =   975
      TabIndex        =   23
      Top             =   4515
      Width           =   885
   End
   Begin VB.CommandButton cmdOk 
      Caption         =   "&Ok"
      Height          =   360
      Left            =   90
      TabIndex        =   22
      Top             =   4515
      Width           =   885
   End
   Begin VB.PictureBox picConnecting 
      BorderStyle     =   0  'None
      Height          =   3465
      Left            =   1980
      ScaleHeight     =   3465
      ScaleWidth      =   3630
      TabIndex        =   17
      Top             =   390
      Width           =   3630
      Begin VB.TextBox txtPort 
         Height          =   315
         Left            =   2955
         MaxLength       =   5
         TabIndex        =   27
         Text            =   "6667"
         Top             =   1830
         Width           =   600
      End
      Begin VB.TextBox txtRetry 
         BackColor       =   &H8000000F&
         BorderStyle     =   0  'None
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   210
         Left            =   1650
         MaxLength       =   2
         TabIndex        =   25
         Text            =   "99"
         Top             =   3135
         Width           =   225
      End
      Begin VB.CheckBox chkRetry 
         Appearance      =   0  'Flat
         Caption         =   " &Retry Connect        times "
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   210
         TabIndex        =   9
         Top             =   3120
         Width           =   3240
      End
      Begin VB.TextBox txtIdent 
         Height          =   315
         Left            =   1530
         TabIndex        =   4
         Text            =   "~IDENT"
         Top             =   1185
         Width           =   2025
      End
      Begin VB.CheckBox chkInvisible 
         Appearance      =   0  'Flat
         Caption         =   " Invisible Mode "
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   210
         TabIndex        =   8
         Top             =   2835
         Width           =   3240
      End
      Begin VB.CheckBox chkReconnect 
         Appearance      =   0  'Flat
         Caption         =   " Reconnect to server on disconnect "
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   210
         TabIndex        =   7
         Top             =   2550
         Width           =   3240
      End
      Begin VB.CheckBox chkStartUp 
         Appearance      =   0  'Flat
         Caption         =   " Connect to server on Client load "
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   210
         TabIndex        =   6
         Top             =   2265
         Width           =   3240
      End
      Begin VB.ComboBox cbServer 
         Height          =   315
         Left            =   210
         Sorted          =   -1  'True
         TabIndex        =   5
         Text            =   "irc.otherside.com"
         Top             =   1830
         Width           =   2685
      End
      Begin VB.TextBox txtFullName 
         Height          =   315
         Left            =   1530
         TabIndex        =   3
         Text            =   "projectIRC User"
         Top             =   810
         Width           =   2025
      End
      Begin VB.TextBox txtOtherNick 
         Height          =   315
         Left            =   1530
         TabIndex        =   2
         Text            =   "OtherNick"
         Top             =   435
         Width           =   2025
      End
      Begin VB.TextBox txtNick 
         Height          =   315
         Left            =   1530
         TabIndex        =   1
         Text            =   "pIRCu"
         Top             =   60
         Width           =   2025
      End
      Begin VB.Label Label6 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Port:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   210
         Left            =   2790
         TabIndex        =   26
         Top             =   1545
         Width           =   390
      End
      Begin VB.Label Label5 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "IDENT:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   210
         Left            =   60
         TabIndex        =   24
         Top             =   1215
         Width           =   495
      End
      Begin VB.Label Label4 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Server:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   210
         Left            =   60
         TabIndex        =   21
         Top             =   1545
         Width           =   600
      End
      Begin VB.Label Label3 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Full Name:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   210
         Left            =   60
         TabIndex        =   20
         Top             =   840
         Width           =   840
      End
      Begin VB.Label Label2 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Alternate Nick:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   210
         Left            =   60
         TabIndex        =   19
         Top             =   465
         Width           =   1200
      End
      Begin VB.Label Label1 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Nick:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   210
         Left            =   60
         TabIndex        =   18
         Top             =   90
         Width           =   390
      End
   End
   Begin VB.CheckBox chkOP 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00800000&
      Caption         =   " DCC... "
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   300
      Index           =   4
      Left            =   135
      TabIndex        =   16
      Top             =   1785
      Width           =   1680
   End
   Begin VB.CheckBox chkOP 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00800000&
      Caption         =   " Display... "
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   300
      Index           =   3
      Left            =   135
      TabIndex        =   15
      Top             =   1470
      Width           =   1680
   End
   Begin VB.CheckBox chkOP 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00800000&
      Caption         =   " Sounds... "
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   300
      Index           =   2
      Left            =   135
      TabIndex        =   14
      Top             =   1155
      Width           =   1680
   End
   Begin VB.CheckBox chkOP 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00800000&
      Caption         =   " IRC... "
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   300
      Index           =   1
      Left            =   135
      TabIndex        =   13
      Top             =   840
      Width           =   1680
   End
   Begin VB.CheckBox chkOP 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00800000&
      Caption         =   " Connecting... "
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   300
      Index           =   0
      Left            =   135
      TabIndex        =   11
      Top             =   525
      Value           =   1  'Checked
      Width           =   1680
   End
   Begin VB.PictureBox picWinIface 
      BorderStyle     =   0  'None
      Height          =   4485
      Left            =   2025
      ScaleHeight     =   4485
      ScaleWidth      =   3600
      TabIndex        =   47
      Top             =   435
      Visible         =   0   'False
      Width           =   3600
      Begin VB.CheckBox chkStretch 
         Appearance      =   0  'Flat
         Caption         =   " &Stretch taskbar buttons to fit taskbar "
         ForeColor       =   &H80000008&
         Height          =   225
         Left            =   60
         TabIndex        =   48
         Top             =   60
         Width           =   3150
      End
   End
   Begin VB.PictureBox picDisplay 
      BorderStyle     =   0  'None
      Height          =   4500
      Left            =   1965
      ScaleHeight     =   4500
      ScaleWidth      =   3675
      TabIndex        =   28
      Top             =   390
      Width           =   3675
      Begin VB.ComboBox cmbFontSize 
         Height          =   315
         Left            =   2880
         TabIndex        =   53
         Top             =   1575
         Width           =   675
      End
      Begin VB.ComboBox cmbFontName 
         Height          =   315
         Left            =   780
         Sorted          =   -1  'True
         Style           =   2  'Dropdown List
         TabIndex        =   52
         Top             =   1575
         Width           =   2085
      End
      Begin VB.CommandButton cmdLBDef 
         Caption         =   "Def"
         Height          =   345
         Left            =   1740
         TabIndex        =   45
         Top             =   765
         Width           =   450
      End
      Begin VB.CommandButton cmdFGDef 
         Caption         =   "Def"
         Height          =   345
         Left            =   1740
         TabIndex        =   44
         Top             =   420
         Width           =   450
      End
      Begin VB.CommandButton cmdBGDef 
         Caption         =   "Def"
         Height          =   345
         Left            =   1740
         TabIndex        =   43
         Top             =   75
         Width           =   450
      End
      Begin VB.CommandButton cmdRCDef 
         Caption         =   "Def"
         Height          =   345
         Left            =   1740
         TabIndex        =   42
         Top             =   1110
         Width           =   450
      End
      Begin VB.PictureBox picRightColor 
         BackColor       =   &H00FFFFFF&
         Enabled         =   0   'False
         Height          =   180
         Left            =   2280
         ScaleHeight     =   120
         ScaleWidth      =   255
         TabIndex        =   39
         Top             =   1200
         Width           =   315
      End
      Begin VB.CommandButton cmdRightColor 
         Caption         =   "       Change..."
         Height          =   345
         Left            =   2190
         TabIndex        =   40
         Top             =   1110
         Width           =   1350
      End
      Begin VB.PictureBox picLeftColor 
         BackColor       =   &H00FFFFFF&
         Enabled         =   0   'False
         Height          =   180
         Left            =   2280
         ScaleHeight     =   120
         ScaleWidth      =   255
         TabIndex        =   36
         Top             =   855
         Width           =   315
      End
      Begin VB.PictureBox picForeColor 
         BackColor       =   &H00FFFFFF&
         Enabled         =   0   'False
         Height          =   180
         Left            =   2280
         ScaleHeight     =   120
         ScaleWidth      =   255
         TabIndex        =   30
         Top             =   510
         Width           =   315
      End
      Begin VB.PictureBox picBackColor 
         BackColor       =   &H00FFFFFF&
         Enabled         =   0   'False
         Height          =   180
         Left            =   2280
         ScaleHeight     =   120
         ScaleWidth      =   255
         TabIndex        =   32
         Top             =   165
         Width           =   315
      End
      Begin VB.CommandButton cmdLeftColor 
         Caption         =   "       Change..."
         Height          =   345
         Left            =   2190
         TabIndex        =   37
         Top             =   765
         Width           =   1350
      End
      Begin VB.CommandButton cmdBGColor 
         Caption         =   "       Change..."
         Height          =   345
         Left            =   2190
         TabIndex        =   33
         Top             =   75
         Width           =   1350
      End
      Begin VB.CommandButton cmdForeColor 
         Caption         =   "       Change..."
         Height          =   345
         Left            =   2190
         TabIndex        =   31
         Top             =   420
         Width           =   1350
      End
      Begin MSComDlg.CommonDialog CDialog 
         Left            =   3720
         Top             =   3525
         _ExtentX        =   847
         _ExtentY        =   847
         _Version        =   393216
         CancelError     =   -1  'True
      End
      Begin VB.Label Label12 
         Caption         =   "Font:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   210
         Left            =   135
         TabIndex        =   51
         Top             =   1620
         Width           =   1305
      End
      Begin VB.Label Label11 
         AutoSize        =   -1  'True
         Caption         =   "Right Back Color:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   210
         Left            =   135
         TabIndex        =   41
         Top             =   1170
         Width           =   1395
      End
      Begin VB.Label Label10 
         AutoSize        =   -1  'True
         Caption         =   "Left Back Color:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   210
         Left            =   135
         TabIndex        =   38
         Top             =   825
         Width           =   1305
      End
      Begin VB.Label Label8 
         AutoSize        =   -1  'True
         Caption         =   "BackGround Color:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   210
         Left            =   135
         TabIndex        =   35
         Top             =   135
         Width           =   1545
      End
      Begin VB.Label Label9 
         AutoSize        =   -1  'True
         Caption         =   "ForeGround Color:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   210
         Left            =   135
         TabIndex        =   34
         Top             =   480
         Width           =   1530
      End
   End
   Begin VB.Shape Shape2 
      BackColor       =   &H8000000F&
      BackStyle       =   1  'Opaque
      BorderStyle     =   0  'Transparent
      Height          =   330
      Left            =   1935
      Shape           =   2  'Oval
      Top             =   345
      Width           =   360
   End
   Begin VB.Shape shpCurve 
      BackColor       =   &H00800000&
      BackStyle       =   1  'Opaque
      BorderStyle     =   0  'Transparent
      Height          =   180
      Left            =   1830
      Top             =   300
      Width           =   240
   End
   Begin VB.Label Label7 
      Caption         =   "Label7"
      Height          =   495
      Left            =   2205
      TabIndex        =   29
      Top             =   1680
      Width           =   1215
   End
   Begin VB.Label lblConnecting 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Connecting..."
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   240
      Left            =   195
      TabIndex        =   12
      Top             =   555
      Width           =   1590
   End
   Begin VB.Label lblWhich 
      AutoSize        =   -1  'True
      BackColor       =   &H00FFFFFF&
      BackStyle       =   0  'Transparent
      Caption         =   "Connecting..."
      ForeColor       =   &H00E0E0E0&
      Height          =   225
      Left            =   1950
      TabIndex        =   0
      Top             =   75
      Width           =   3285
   End
   Begin VB.Label lbl1 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Options..."
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   210
      Left            =   90
      TabIndex        =   10
      Top             =   75
      Width           =   1800
   End
   Begin VB.Shape Shape1 
      BackColor       =   &H00800000&
      BackStyle       =   1  'Opaque
      BorderStyle     =   0  'Transparent
      Height          =   360
      Left            =   -210
      Top             =   0
      Width           =   5955
   End
   Begin VB.Shape shpBlue 
      BackColor       =   &H00800000&
      BackStyle       =   1  'Opaque
      BorderStyle     =   0  'Transparent
      Height          =   5010
      Left            =   0
      Top             =   0
      Width           =   1950
   End
End
Attribute VB_Name = "Options"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim bC As Boolean
Sub AddServer(strName As String, bAppend As Boolean)
    If strName = "" Then Exit Sub
    Dim lngRet As Long
    lngRet = SendMessage(cbServer.hwnd, CB_FINDSTRINGEXACT, -1, ByVal strName)
    
    If lngRet <> -1 Then Exit Sub
    cbServer.AddItem strName
    If Not bAppend Then Exit Sub
    Open path & "serverlist.data" For Append As #1
        Print #1, strName
    Close #1
End Sub


Sub HideAll()
    picConnecting.Visible = False
    picDisplay.Visible = False
    picWinIface.Visible = False
    picIRC.Visible = False
End Sub


Sub LoadFonts()
    Dim i As Integer
    For i = 0 To Screen.FontCount
        cmbFontName.AddItem Screen.Fonts(i)
    Next i
    For i = 0 To cmbFontName.ListCount - 1
        If LCase(cmbFontName.List(i)) = LCase(strFontName) Then
            cmbFontName.ListIndex = i
            Exit For
        End If
    Next i
    cmbFontSize.Text = intFontSize
End Sub

Sub SaveConnect()
    strMyNick = txtNick         'nick
    WriteINI "connect", "nick", strMyNick
    strOtherNick = txtOtherNick 'altnick
    WriteINI "connect", "altnick", strOtherNick
    strFullName = txtFullName   'full name
    WriteINI "connect", "fullname", strFullName
    strMyIdent = txtIdent       'ident
    WriteINI "connect", "ident", strMyIdent
    strServer = cbServer.Text   'server
    WriteINI "connect", "server", strServer
    lngPort = CLng(txtPort)     'port
    WriteINI "connect", "port", CStr(lngPort)
    bConOnLoad = chkStartUp     'connect on load
    WriteINI "connect", "connonload", CStr(bConOnLoad)
    bReconnect = chkReconnect   'reconnect on disconnect
    WriteINI "connect", "reconnect", CStr(bReconnect)
    bInvisible = chkInvisible   'invisible mode
    WriteINI "connect", "invisible", CStr(bInvisible)
    bRetry = chkRetry           'retry connect
    WriteINI "connect", "retry", CStr(bRetry)
    intRetry = CInt(txtRetry)   'number of retries
    WriteINI "connect", "retrynum", CStr(intRetry)
End Sub
Sub SaveDisplay()
    lngBackColor = picBackColor.BackColor
    WriteINI "display", "backcolor", CStr(lngBackColor)
    lngForeColor = picForeColor.BackColor
    WriteINI "display", "forecolor", CStr(lngForeColor)
    lngLeftColor = picLeftColor.BackColor
    WriteINI "display", "leftcolor", CStr(lngLeftColor)
    lngRightColor = picRightColor.BackColor
    WriteINI "display", "rightcolor", CStr(lngRightColor)
    strFontName = cmbFontName.Text
    WriteINI "display", "fontname", strFontName
    intFontSize = CInt(cmbFontSize.Text)
    WriteINI "display", "fontsize", CStr(intFontSize)
End Sub


Sub SaveIRC()
    bWhoisInQuery = chkWhoIsQ   'do whois in query
    WriteINI "irc", "whoisquery", CStr(bWhoisInQuery)
    bRejoinOnKick = chkRejoin   'rejoin channel on kick
    WriteINI "irc", "rejoinonkick", CStr(bRejoinOnKick)
    bHidePing = chkHidePing
    WriteINI "irc", "hideping", CStr(bHidePing)
End Sub

Sub SaveWinIface()
    bStretchButtons = chkStretch     'retry connect
    WriteINI "windows", "stretch", CStr(bStretchButtons)
End Sub

Private Sub cbServer_GotFocus()
    With cbServer
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub


Private Sub cbServer_KeyPress(KeyAscii As Integer)
    If KeyAscii = 13 Then
        AddServer cbServer.Text, True
        txtPort.SetFocus
    End If
End Sub


Private Sub chkOP_MouseDown(Index As Integer, Button As Integer, Shift As Integer, x As Single, Y As Single)
    For i = 0 To 5
        'If i <> Index And chkOP(i).Value = 1 Then
        If i <> Index And chkOP(i).Value = 1 Then chkOP(i).Value = 0
    Next i
    chkOP(Index).Value = 1
    Select Case Index
        Case 0  'connecting
            HideAll
            picConnecting.Visible = True
            lblWhich.Caption = "Connection Settings"
        Case 1
            HideAll
            picIRC.Visible = True
            lblWhich.Caption = "IRC Settings"
        
        
        Case 3  'display
            HideAll
            picDisplay.Visible = True
            lblWhich.Caption = "Display Settings"
        Case 5  'windows/iface
            HideAll
            picWinIface.Visible = True
            lblWhich.Caption = "Windows/Interface Settings"
    End Select
End Sub


Private Sub chkOP_MouseUp(Index As Integer, Button As Integer, Shift As Integer, x As Single, Y As Single)
    For i = 0 To 4
        'If i <> Index And chkOP(i).Value = 1 Then
        If i <> Index And chkOP(i).Value = 1 Then chkOP(i).Value = 0
    Next i
    chkOP(Index).Value = 1
End Sub


Private Sub cmdBGColor_Click()
    On Error Resume Next
    CDialog.Color = picBackColor.BackColor
    CDialog.ShowColor
    If Err Then Exit Sub
    picBackColor.BackColor = CDialog.Color
End Sub

Private Sub cmdBGDef_Click()
    picBackColor.BackColor = RGB(255, 255, 255)
End Sub

Private Sub cmdCancel_Click()
    Unload Me
End Sub

Private Sub cmdFGDef_Click()
    picForeColor.BackColor = 0
End Sub

Private Sub cmdForeColor_Click()
    On Error Resume Next
    CDialog.Color = picForeColor.BackColor
    CDialog.ShowColor
    If Err Then Exit Sub
    picForeColor.BackColor = CDialog.Color

End Sub

Private Sub cmdLBDef_Click()
    picLeftColor.BackColor = &H800000
End Sub

Private Sub cmdLeftColor_Click()
    On Error Resume Next
    CDialog.Color = picLeftColor.BackColor
    CDialog.ShowColor
    If Err Then Exit Sub
    picLeftColor.BackColor = CDialog.Color

End Sub


Private Sub cmdOk_Click()
    SaveConnect
    SaveDisplay
    SaveWinIface
    SaveIRC
    Unload Me
End Sub

Private Sub cmdRCDef_Click()
    picRightColor.BackColor = &H8000000F
End Sub

Private Sub cmdRightColor_Click()
    On Error Resume Next
    CDialog.Color = picRightColor.BackColor
    CDialog.ShowColor
    If Err Then Exit Sub
    picRightColor.BackColor = CDialog.Color

End Sub

Private Sub Form_Load()
    HideAll
    picConnecting.Visible = True
    
    '* Connection settings
    txtNick = strMyNick
    txtOtherNick = strOtherNick
    txtFullName = strFullName
    txtIdent = strMyIdent
    txtPort = CStr(lngPort)
    cbServer.Text = strServer
    chkStartUp = TF(bConOnLoad)
    chkReconnect.Value = TF(bReconnect)
    chkInvisible.Value = TF(bInvisible)
    chkRetry.Value = TF(bRetry)
    txtRetry = CInt(intRetry)
    
    '* Display settings
    picBackColor.BackColor = CLng(lngBackColor)
    picForeColor.BackColor = CLng(lngForeColor)
    picLeftColor.BackColor = CLng(lngLeftColor)
    picRightColor.BackColor = CLng(lngRightColor)
    lbl1.ForeColor = CLng(lngBackColor)
    For i = 0 To 4
        chkOP(i).ForeColor = CLng(lngBackColor)
    Next i
    
    '* Windows/Interface
    chkStretch.Value = TF(bStretchButtons)
    
    '* IRC
    chkWhoIsQ.Value = TF(bWhoisInQuery)
    chkRejoin.Value = TF(bRejoinOnKick)
    chkHidePing.Value = TF(bHidePing)
    
    '* Blah, servers
    Dim srvlst As String, strData As String, strList() As String
    srvlst = path & "serverlist.data"
    If Not FileExists(srvlst) Then Open srvlst For Output As #1: Print #1, "": Close #1
    
    Open srvlst For Binary As #1
        strData = String(LOF(1), 0)
        Get #1, 1, strData
    Close #1
    strList = Split(strData, vbCrLf)
    strData = ""
    For i = LBound(strList) To UBound(strList)
        AddServer strList(i), False
    Next i
    
    LoadFonts
    
End Sub

Private Sub txtFullName_GotFocus()
    With txtFullName
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub


Private Sub txtFullName_KeyPress(KeyAscii As Integer)
    If KeyAscii = 13 Then txtIdent.SetFocus
End Sub


Private Sub txtIdent_GotFocus()
    With txtIdent
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub


Private Sub txtIdent_KeyPress(KeyAscii As Integer)
    If KeyAscii = 13 Then cbServer.SetFocus
End Sub


Private Sub txtNick_GotFocus()
    With txtNick
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub


Private Sub txtNick_KeyPress(KeyAscii As Integer)
    If KeyAscii = 13 Then txtOtherNick.SetFocus
    If KeyAscii = Asc(" ") Then KeyAscii = 0
End Sub


Private Sub txtOtherNick_GotFocus()
    With txtOtherNick
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub


Private Sub txtOtherNick_KeyPress(KeyAscii As Integer)
    If KeyAscii = 13 Then txtFullName.SetFocus
    If KeyAscii = Asc(" ") Then KeyAscii = 0
End Sub


Private Sub txtPort_GotFocus()
    With txtPort
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub

Private Sub txtPort_KeyPress(KeyAscii As Integer)
    If KeyAscii = 13 Then cmdOk_Click
End Sub


Private Sub txtRetry_KeyPress(KeyAscii As Integer)
    If IsNumeric(Chr(KeyAscii)) Then Else KeyAscii = 0
End Sub


