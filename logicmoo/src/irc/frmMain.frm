VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "tabctl32.ocx"
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "DalNET Services Command Center"
   ClientHeight    =   4470
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7095
   Icon            =   "frmMain.frx":0000
   LinkMode        =   1  'Source
   LinkTopic       =   "frmMain"
   MaxButton       =   0   'False
   ScaleHeight     =   4470
   ScaleWidth      =   7095
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton cmdDelSettings 
      Caption         =   "&Delete Settings"
      Height          =   375
      Left            =   3000
      TabIndex        =   82
      Top             =   3960
      Width           =   1335
   End
   Begin VB.CommandButton cmdSave 
      Caption         =   "S&ave Settings"
      Height          =   375
      Left            =   1560
      TabIndex        =   81
      Top             =   3960
      Width           =   1335
   End
   Begin VB.CommandButton cmdGetSettings 
      Caption         =   "&Get Settings"
      Height          =   375
      Left            =   120
      TabIndex        =   80
      Top             =   3960
      Width           =   1335
   End
   Begin VB.TextBox txtMemo 
      Height          =   285
      Left            =   120
      LinkTimeout     =   5
      TabIndex        =   6
      Top             =   3000
      Width           =   6855
   End
   Begin VB.TextBox txtOutput 
      Height          =   285
      Left            =   120
      LinkTimeout     =   5
      LinkTopic       =   "mIRC|command"
      TabIndex        =   7
      Top             =   3600
      Width           =   6855
   End
   Begin VB.CommandButton cmdExit 
      Caption         =   "&Exit"
      Height          =   375
      Left            =   5760
      TabIndex        =   9
      Top             =   3960
      Width           =   1215
   End
   Begin TabDlg.SSTab tabMain 
      Height          =   2535
      Left            =   120
      TabIndex        =   4
      Top             =   120
      Width           =   6855
      _ExtentX        =   12091
      _ExtentY        =   4471
      _Version        =   393216
      Tabs            =   4
      Tab             =   2
      TabsPerRow      =   4
      TabHeight       =   441
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Courier New"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      TabCaption(0)   =   "&ChanServ"
      TabPicture(0)   =   "frmMain.frx":0442
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "tabChanServ"
      Tab(0).ControlCount=   1
      TabCaption(1)   =   "&MemoServ"
      TabPicture(1)   =   "frmMain.frx":045E
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "optFwdOff"
      Tab(1).Control(1)=   "optFwdOn"
      Tab(1).Control(2)=   "optFwdStatus"
      Tab(1).Control(3)=   "optPurge"
      Tab(1).Control(4)=   "optUnDelete"
      Tab(1).Control(5)=   "optDelete"
      Tab(1).Control(6)=   "optSendSOP"
      Tab(1).Control(7)=   "optSend"
      Tab(1).Control(8)=   "optRead"
      Tab(1).Control(9)=   "optList"
      Tab(1).ControlCount=   10
      TabCaption(2)   =   "N&ickServ"
      TabPicture(2)   =   "frmMain.frx":047A
      Tab(2).ControlEnabled=   -1  'True
      Tab(2).Control(0)=   "tabNickserv"
      Tab(2).Control(0).Enabled=   0   'False
      Tab(2).ControlCount=   1
      TabCaption(3)   =   "&User Settings"
      TabPicture(3)   =   "frmMain.frx":0496
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "txtPassword"
      Tab(3).Control(1)=   "txtChannel"
      Tab(3).Control(2)=   "chkTransparent"
      Tab(3).Control(3)=   "txtNickname"
      Tab(3).Control(4)=   "lblPassword"
      Tab(3).Control(5)=   "lblChannel"
      Tab(3).Control(6)=   "lblNickname"
      Tab(3).ControlCount=   7
      Begin VB.TextBox txtPassword 
         Height          =   285
         IMEMode         =   3  'DISABLE
         Left            =   -71640
         PasswordChar    =   "*"
         TabIndex        =   1
         Top             =   840
         Width           =   1335
      End
      Begin VB.TextBox txtChannel 
         Height          =   285
         Left            =   -71640
         TabIndex        =   2
         Top             =   1200
         Width           =   1335
      End
      Begin VB.CheckBox chkTransparent 
         Caption         =   "Make program transparent.  This works in Windows 2000 only."
         Height          =   285
         Left            =   -73830
         TabIndex        =   3
         Top             =   1680
         Width           =   4755
      End
      Begin VB.TextBox txtNickname 
         Height          =   285
         Left            =   -71640
         TabIndex        =   0
         Top             =   480
         Width           =   1335
      End
      Begin TabDlg.SSTab tabChanServ 
         Height          =   2055
         Left            =   -74760
         TabIndex        =   46
         Top             =   360
         Width           =   6375
         _ExtentX        =   11245
         _ExtentY        =   3625
         _Version        =   393216
         Tab             =   2
         TabHeight       =   441
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Courier New"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         TabCaption(0)   =   "Ge&neral Commands"
         TabPicture(0)   =   "frmMain.frx":04B2
         Tab(0).ControlEnabled=   0   'False
         Tab(0).Control(0)=   "optChanUnBan"
         Tab(0).Control(1)=   "optChanDeop"
         Tab(0).Control(2)=   "optChanMDEOP"
         Tab(0).Control(3)=   "optChanMKick"
         Tab(0).Control(4)=   "optChanInvite"
         Tab(0).Control(5)=   "optChanInfo"
         Tab(0).Control(6)=   "optChanWhy"
         Tab(0).Control(7)=   "optChanCount"
         Tab(0).Control(8)=   "optChanAccess"
         Tab(0).Control(9)=   "optChanID"
         Tab(0).Control(10)=   "optChanDrop"
         Tab(0).Control(11)=   "optChanReg"
         Tab(0).ControlCount=   12
         TabCaption(1)   =   "&OP List Management"
         TabPicture(1)   =   "frmMain.frx":04CE
         Tab(1).ControlEnabled=   0   'False
         Tab(1).Control(0)=   "optChanSOPList"
         Tab(1).Control(1)=   "optChanSOPAdd"
         Tab(1).Control(2)=   "optChanSOPDel"
         Tab(1).Control(3)=   "optChanSOPWipe"
         Tab(1).Control(4)=   "optChanSOPClean"
         Tab(1).Control(5)=   "optChanAOPList"
         Tab(1).Control(6)=   "optChanAOPAdd"
         Tab(1).Control(7)=   "optChanAOPDel"
         Tab(1).Control(8)=   "optChanAOPWipe"
         Tab(1).Control(9)=   "optChanAOPClean"
         Tab(1).Control(10)=   "optChanAKickList"
         Tab(1).Control(11)=   "optChanAKickAdd"
         Tab(1).Control(12)=   "optChanAKickDel"
         Tab(1).Control(13)=   "optChanAKickWipe"
         Tab(1).Control(14)=   "optChanOpUser"
         Tab(1).Control(15)=   "optChanDeopUser"
         Tab(1).ControlCount=   16
         TabCaption(2)   =   "Se&t Commands"
         TabPicture(2)   =   "frmMain.frx":04EA
         Tab(2).ControlEnabled=   -1  'True
         Tab(2).Control(0)=   "tabChanSet"
         Tab(2).Control(0).Enabled=   0   'False
         Tab(2).ControlCount=   1
         Begin TabDlg.SSTab tabChanSet 
            Height          =   1575
            Left            =   120
            TabIndex        =   83
            Top             =   360
            Width           =   6135
            _ExtentX        =   10821
            _ExtentY        =   2778
            _Version        =   393216
            Tabs            =   2
            TabsPerRow      =   2
            TabHeight       =   441
            BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
               Name            =   "Courier New"
               Size            =   9
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            TabCaption(0)   =   "General"
            TabPicture(0)   =   "frmMain.frx":0506
            Tab(0).ControlEnabled=   -1  'True
            Tab(0).Control(0)=   "Label1"
            Tab(0).Control(0).Enabled=   0   'False
            Tab(0).Control(1)=   "Label2"
            Tab(0).Control(1).Enabled=   0   'False
            Tab(0).Control(2)=   "optSetChanFounder"
            Tab(0).Control(2).Enabled=   0   'False
            Tab(0).Control(3)=   "optSetChanPass"
            Tab(0).Control(3).Enabled=   0   'False
            Tab(0).Control(4)=   "optSetChanDesc"
            Tab(0).Control(4).Enabled=   0   'False
            Tab(0).Control(5)=   "optSetChanMode"
            Tab(0).Control(5).Enabled=   0   'False
            Tab(0).Control(6)=   "optSetChanURL"
            Tab(0).Control(6).Enabled=   0   'False
            Tab(0).Control(7)=   "optSetChanTLFounder"
            Tab(0).Control(7).Enabled=   0   'False
            Tab(0).Control(8)=   "optSetChanTLSOP"
            Tab(0).Control(8).Enabled=   0   'False
            Tab(0).Control(9)=   "optSetChanTLAOP"
            Tab(0).Control(9).Enabled=   0   'False
            Tab(0).Control(10)=   "optSetChanMemoFounder"
            Tab(0).Control(10).Enabled=   0   'False
            Tab(0).Control(11)=   "optSetChanMemoSOP"
            Tab(0).Control(11).Enabled=   0   'False
            Tab(0).Control(12)=   "optSetChanMemoAOP"
            Tab(0).Control(12).Enabled=   0   'False
            Tab(0).Control(13)=   "optSetChanMemoNone"
            Tab(0).Control(13).Enabled=   0   'False
            Tab(0).ControlCount=   14
            TabCaption(1)   =   "Switched"
            TabPicture(1)   =   "frmMain.frx":0522
            Tab(1).ControlEnabled=   0   'False
            Tab(1).Control(0)=   "Label3"
            Tab(1).Control(1)=   "Label4"
            Tab(1).Control(2)=   "Label5"
            Tab(1).Control(3)=   "Label6"
            Tab(1).Control(4)=   "Label7"
            Tab(1).Control(5)=   "Label8"
            Tab(1).Control(6)=   "Label9"
            Tab(1).Control(7)=   "optSetChanOpGrdON"
            Tab(1).Control(8)=   "optSetChanOpGrdOFF"
            Tab(1).Control(9)=   "optSetChanKTON"
            Tab(1).Control(10)=   "optSetChanKTOFF"
            Tab(1).Control(11)=   "optSetChanOPIDON"
            Tab(1).Control(12)=   "optSetChanOPIDOFF"
            Tab(1).Control(13)=   "optSetChanRestON"
            Tab(1).Control(14)=   "optSetChanRestOFF"
            Tab(1).Control(15)=   "optSetChanLopON"
            Tab(1).Control(16)=   "optSetChanLopOFF"
            Tab(1).Control(17)=   "optSetChanUnSecON"
            Tab(1).Control(18)=   "optSetChanUnSecOFF"
            Tab(1).Control(19)=   "optSetChanPvtON"
            Tab(1).Control(20)=   "optSetChanPvtOFF"
            Tab(1).ControlCount=   21
            Begin VB.OptionButton optSetChanPvtOFF 
               Caption         =   "OFF"
               Height          =   255
               Left            =   -69720
               TabIndex        =   118
               Top             =   840
               Width           =   615
            End
            Begin VB.OptionButton optSetChanPvtON 
               Caption         =   "ON"
               Height          =   255
               Left            =   -70440
               TabIndex        =   117
               Top             =   840
               Width           =   615
            End
            Begin VB.OptionButton optSetChanUnSecOFF 
               Caption         =   "OFF"
               Height          =   255
               Left            =   -69720
               TabIndex        =   116
               Top             =   600
               Width           =   615
            End
            Begin VB.OptionButton optSetChanUnSecON 
               Caption         =   "ON"
               Height          =   255
               Left            =   -70440
               TabIndex        =   115
               Top             =   600
               Width           =   615
            End
            Begin VB.OptionButton optSetChanLopOFF 
               Caption         =   "OFF"
               Height          =   255
               Left            =   -69720
               TabIndex        =   114
               Top             =   360
               Width           =   615
            End
            Begin VB.OptionButton optSetChanLopON 
               Caption         =   "ON"
               Height          =   255
               Left            =   -70440
               TabIndex        =   113
               Top             =   360
               Width           =   615
            End
            Begin VB.OptionButton optSetChanRestOFF 
               Caption         =   "OFF"
               Height          =   255
               Left            =   -73200
               TabIndex        =   109
               Top             =   1080
               Width           =   615
            End
            Begin VB.OptionButton optSetChanRestON 
               Caption         =   "ON"
               Height          =   255
               Left            =   -73920
               TabIndex        =   108
               Top             =   1080
               Width           =   615
            End
            Begin VB.OptionButton optSetChanOPIDOFF 
               Caption         =   "OFF"
               Height          =   255
               Left            =   -73200
               TabIndex        =   107
               Top             =   840
               Width           =   615
            End
            Begin VB.OptionButton optSetChanOPIDON 
               Caption         =   "ON"
               Height          =   255
               Left            =   -73920
               TabIndex        =   106
               Top             =   840
               Width           =   615
            End
            Begin VB.OptionButton optSetChanKTOFF 
               Caption         =   "OFF"
               Height          =   255
               Left            =   -73200
               TabIndex        =   105
               Top             =   600
               Width           =   615
            End
            Begin VB.OptionButton optSetChanKTON 
               Caption         =   "ON"
               Height          =   255
               Left            =   -73920
               TabIndex        =   104
               Top             =   600
               Width           =   615
            End
            Begin VB.OptionButton optSetChanOpGrdOFF 
               Caption         =   "OFF"
               Height          =   255
               Left            =   -73200
               TabIndex        =   103
               Top             =   360
               Width           =   615
            End
            Begin VB.OptionButton optSetChanOpGrdON 
               Caption         =   "ON"
               Height          =   255
               Left            =   -73920
               TabIndex        =   102
               Top             =   360
               Width           =   615
            End
            Begin VB.OptionButton optSetChanMemoNone 
               Caption         =   "None"
               Height          =   255
               Left            =   5280
               TabIndex        =   95
               Top             =   1200
               Width           =   735
            End
            Begin VB.OptionButton optSetChanMemoAOP 
               Caption         =   "AOP"
               Height          =   255
               Left            =   4440
               TabIndex        =   94
               Top             =   1200
               Width           =   735
            End
            Begin VB.OptionButton optSetChanMemoSOP 
               Caption         =   "SOP"
               Height          =   255
               Left            =   3600
               TabIndex        =   93
               Top             =   1200
               Width           =   735
            End
            Begin VB.OptionButton optSetChanMemoFounder 
               Caption         =   "Founder"
               Height          =   255
               Left            =   2520
               TabIndex        =   92
               Top             =   1200
               Width           =   975
            End
            Begin VB.OptionButton optSetChanTLAOP 
               Caption         =   "AOP"
               Height          =   255
               Left            =   5040
               TabIndex        =   91
               Top             =   600
               Width           =   735
            End
            Begin VB.OptionButton optSetChanTLSOP 
               Caption         =   "SOP"
               Height          =   255
               Left            =   4200
               TabIndex        =   90
               Top             =   600
               Width           =   735
            End
            Begin VB.OptionButton optSetChanTLFounder 
               Caption         =   "Founder"
               Height          =   255
               Left            =   3120
               TabIndex        =   89
               Top             =   600
               Width           =   975
            End
            Begin VB.OptionButton optSetChanURL 
               Caption         =   "Set Channel URL"
               Height          =   255
               Left            =   120
               TabIndex        =   88
               Top             =   1200
               Width           =   2055
            End
            Begin VB.OptionButton optSetChanMode 
               Caption         =   "Set Channel Modes"
               Height          =   255
               Left            =   120
               TabIndex        =   87
               Top             =   960
               Width           =   2055
            End
            Begin VB.OptionButton optSetChanDesc 
               Caption         =   "Set Channel Description"
               Height          =   255
               Left            =   120
               TabIndex        =   86
               Top             =   720
               Width           =   2055
            End
            Begin VB.OptionButton optSetChanPass 
               Caption         =   "Set Channel Password"
               Height          =   255
               Left            =   120
               TabIndex        =   85
               Top             =   480
               Width           =   2055
            End
            Begin VB.OptionButton optSetChanFounder 
               Caption         =   "Set Channel Founder"
               Height          =   255
               Left            =   120
               TabIndex        =   84
               Top             =   240
               Width           =   2055
            End
            Begin VB.Label Label9 
               Caption         =   "Set Private Channel"
               Height          =   255
               Left            =   -72240
               TabIndex        =   112
               Top             =   840
               Width           =   1695
            End
            Begin VB.Label Label8 
               Caption         =   "Set UnSecure Channel"
               Height          =   255
               Left            =   -72240
               TabIndex        =   111
               Top             =   600
               Width           =   1695
            End
            Begin VB.Label Label7 
               Caption         =   "Set Leave Ops"
               Height          =   255
               Left            =   -72240
               TabIndex        =   110
               Top             =   360
               Width           =   1695
            End
            Begin VB.Label Label6 
               Caption         =   "Restrict"
               Height          =   255
               Left            =   -74880
               TabIndex        =   101
               Top             =   1080
               Width           =   855
            End
            Begin VB.Label Label5 
               Caption         =   "Op ID"
               Height          =   255
               Left            =   -74880
               TabIndex        =   100
               Top             =   840
               Width           =   855
            End
            Begin VB.Label Label4 
               Caption         =   "KeepTopic"
               Height          =   255
               Left            =   -74880
               TabIndex        =   99
               Top             =   600
               Width           =   855
            End
            Begin VB.Label Label3 
               Caption         =   "Op Guard"
               Height          =   255
               Left            =   -74880
               TabIndex        =   98
               Top             =   360
               Width           =   855
            End
            Begin VB.Label Label2 
               Alignment       =   2  'Center
               Caption         =   "Set Topic Lock"
               Height          =   255
               Left            =   3240
               TabIndex        =   97
               Top             =   360
               Width           =   2535
            End
            Begin VB.Label Label1 
               Alignment       =   2  'Center
               Caption         =   "Set Memo Recipients"
               Height          =   255
               Left            =   2520
               TabIndex        =   96
               Top             =   960
               Width           =   3495
            End
         End
         Begin VB.OptionButton optChanDeopUser 
            Caption         =   "Revoke OP from User"
            Height          =   255
            Left            =   -71640
            TabIndex        =   79
            Top             =   360
            Width           =   1935
         End
         Begin VB.OptionButton optChanOpUser 
            Caption         =   "Grant OP to User"
            Height          =   255
            Left            =   -73680
            TabIndex        =   78
            Top             =   360
            Width           =   1695
         End
         Begin VB.OptionButton optChanAKickWipe 
            Caption         =   "Channel AKick Wipe"
            Height          =   255
            Left            =   -70800
            TabIndex        =   77
            Top             =   1440
            Width           =   2055
         End
         Begin VB.OptionButton optChanAKickDel 
            Caption         =   "Channel AKick Delete"
            Height          =   255
            Left            =   -70800
            TabIndex        =   76
            Top             =   1200
            Width           =   2055
         End
         Begin VB.OptionButton optChanAKickAdd 
            Caption         =   "Channel AKick Add"
            Height          =   255
            Left            =   -70800
            TabIndex        =   75
            Top             =   960
            Width           =   2055
         End
         Begin VB.OptionButton optChanAKickList 
            Caption         =   "Channel AKick List"
            Height          =   255
            Left            =   -70800
            TabIndex        =   74
            Top             =   720
            Width           =   2055
         End
         Begin VB.OptionButton optChanAOPClean 
            Caption         =   "Channel AOP Clean"
            Height          =   255
            Left            =   -72840
            TabIndex        =   73
            Top             =   1680
            Width           =   1935
         End
         Begin VB.OptionButton optChanAOPWipe 
            Caption         =   "Channel AOP Wipe"
            Height          =   255
            Left            =   -72840
            TabIndex        =   72
            Top             =   1440
            Width           =   1935
         End
         Begin VB.OptionButton optChanAOPDel 
            Caption         =   "Channel AOP Delete"
            Height          =   255
            Left            =   -72840
            TabIndex        =   71
            Top             =   1200
            Width           =   1935
         End
         Begin VB.OptionButton optChanAOPAdd 
            Caption         =   "Channel AOP Add"
            Height          =   255
            Left            =   -72840
            TabIndex        =   70
            Top             =   960
            Width           =   1935
         End
         Begin VB.OptionButton optChanAOPList 
            Caption         =   "Channel AOP List"
            Height          =   255
            Left            =   -72840
            TabIndex        =   69
            Top             =   720
            Width           =   1935
         End
         Begin VB.OptionButton optChanSOPClean 
            Caption         =   "Channel SOP Clean"
            Height          =   255
            Left            =   -74880
            TabIndex        =   68
            Top             =   1680
            Width           =   1815
         End
         Begin VB.OptionButton optChanSOPWipe 
            Caption         =   "Channel SOP Wipe"
            Height          =   255
            Left            =   -74880
            TabIndex        =   67
            Top             =   1440
            Width           =   1815
         End
         Begin VB.OptionButton optChanSOPDel 
            Caption         =   "Channel SOP Delete"
            Height          =   255
            Left            =   -74880
            TabIndex        =   66
            Top             =   1200
            Width           =   1815
         End
         Begin VB.OptionButton optChanSOPAdd 
            Caption         =   "Channel SOP Add"
            Height          =   255
            Left            =   -74880
            TabIndex        =   65
            Top             =   960
            Width           =   1815
         End
         Begin VB.OptionButton optChanSOPList 
            Caption         =   "Channel SOP List"
            Height          =   255
            Left            =   -74880
            TabIndex        =   64
            Top             =   720
            Width           =   1815
         End
         Begin VB.OptionButton optChanUnBan 
            Caption         =   "UnBan User from Channel"
            Height          =   255
            Left            =   -71280
            TabIndex        =   63
            Top             =   1680
            Width           =   2535
         End
         Begin VB.OptionButton optChanDeop 
            Caption         =   "DEOP User in Channel"
            Height          =   255
            Left            =   -71280
            TabIndex        =   62
            Top             =   1440
            Width           =   2535
         End
         Begin VB.OptionButton optChanMDEOP 
            Caption         =   "Mass DEOP Users in Channel"
            Height          =   255
            Left            =   -71280
            TabIndex        =   61
            Top             =   1200
            Width           =   2535
         End
         Begin VB.OptionButton optChanMKick 
            Caption         =   "Kick All Users From Channel"
            Height          =   255
            Left            =   -71280
            TabIndex        =   60
            Top             =   960
            Width           =   2535
         End
         Begin VB.OptionButton optChanInvite 
            Caption         =   "Invite User Into Channel"
            Height          =   255
            Left            =   -71280
            TabIndex        =   59
            Top             =   720
            Width           =   2535
         End
         Begin VB.OptionButton optChanInfo 
            Caption         =   "Channel Information"
            Height          =   255
            Left            =   -71280
            TabIndex        =   58
            Top             =   480
            Width           =   2415
         End
         Begin VB.OptionButton optChanWhy 
            Caption         =   "Why A Nick Was Given OP"
            Height          =   255
            Left            =   -74880
            TabIndex        =   57
            Top             =   1680
            Width           =   2295
         End
         Begin VB.OptionButton optChanCount 
            Caption         =   "Channel Count (# of Ops)"
            Height          =   255
            Left            =   -74880
            TabIndex        =   56
            Top             =   1440
            Width           =   2295
         End
         Begin VB.OptionButton optChanAccess 
            Caption         =   "Channel Access List"
            Height          =   255
            Left            =   -74880
            TabIndex        =   55
            Top             =   1200
            Width           =   2295
         End
         Begin VB.OptionButton optChanID 
            Caption         =   "Identify to ChanServ"
            Height          =   255
            Left            =   -74880
            TabIndex        =   54
            Top             =   960
            Width           =   2295
         End
         Begin VB.OptionButton optChanDrop 
            Caption         =   "Drop Channel"
            Height          =   255
            Left            =   -74880
            TabIndex        =   53
            Top             =   720
            Width           =   2175
         End
         Begin VB.OptionButton optChanReg 
            Caption         =   "Register New Channel"
            Height          =   255
            Left            =   -74880
            TabIndex        =   52
            Top             =   480
            Width           =   2175
         End
      End
      Begin VB.OptionButton optFwdOff 
         Caption         =   "Disable Forwarding"
         Height          =   255
         Left            =   -71310
         TabIndex        =   45
         Top             =   2070
         Width           =   2055
      End
      Begin VB.OptionButton optFwdOn 
         Caption         =   "Enable Forwarding"
         Height          =   255
         Left            =   -71310
         TabIndex        =   44
         Top             =   1710
         Width           =   2055
      End
      Begin VB.OptionButton optFwdStatus 
         Caption         =   "Forwarding Status"
         Height          =   255
         Left            =   -71310
         TabIndex        =   43
         Top             =   1350
         Width           =   2055
      End
      Begin VB.OptionButton optPurge 
         Caption         =   "Purge Messages"
         Height          =   255
         Left            =   -71310
         TabIndex        =   42
         Top             =   990
         Width           =   2055
      End
      Begin VB.OptionButton optUnDelete 
         Caption         =   "Undelete Messages"
         Height          =   255
         Left            =   -71310
         TabIndex        =   41
         Top             =   630
         Width           =   2055
      End
      Begin VB.OptionButton optDelete 
         Caption         =   "Delete Messages"
         Height          =   255
         Left            =   -74790
         TabIndex        =   40
         Top             =   2070
         Width           =   2415
      End
      Begin VB.OptionButton optSendSOP 
         Caption         =   "Send Message to SOP"
         Height          =   255
         Left            =   -74790
         TabIndex        =   39
         Top             =   1710
         Width           =   2295
      End
      Begin VB.OptionButton optSend 
         Caption         =   "Send Message to Nick"
         Height          =   255
         Left            =   -74790
         TabIndex        =   38
         Top             =   1350
         Width           =   2175
      End
      Begin VB.OptionButton optRead 
         Caption         =   "Read Messages"
         Height          =   255
         Left            =   -74790
         TabIndex        =   37
         Top             =   990
         Width           =   2055
      End
      Begin VB.OptionButton optList 
         Caption         =   "List Messages"
         Height          =   255
         Left            =   -74790
         TabIndex        =   36
         Top             =   630
         Width           =   2055
      End
      Begin TabDlg.SSTab tabNickserv 
         Height          =   1965
         Left            =   240
         TabIndex        =   10
         Top             =   390
         Width           =   6375
         _ExtentX        =   11245
         _ExtentY        =   3466
         _Version        =   393216
         Tabs            =   2
         TabsPerRow      =   2
         TabHeight       =   441
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Courier New"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         TabCaption(0)   =   "&General Commands"
         TabPicture(0)   =   "frmMain.frx":053E
         Tab(0).ControlEnabled=   -1  'True
         Tab(0).Control(0)=   "optRegister"
         Tab(0).Control(0).Enabled=   0   'False
         Tab(0).Control(1)=   "optIdentify"
         Tab(0).Control(1).Enabled=   0   'False
         Tab(0).Control(2)=   "optRecover"
         Tab(0).Control(2).Enabled=   0   'False
         Tab(0).Control(3)=   "optGhost"
         Tab(0).Control(3).Enabled=   0   'False
         Tab(0).Control(4)=   "optRelease"
         Tab(0).Control(4).Enabled=   0   'False
         Tab(0).Control(5)=   "optInfo"
         Tab(0).Control(5).Enabled=   0   'False
         Tab(0).Control(6)=   "optDrop"
         Tab(0).Control(6).Enabled=   0   'False
         Tab(0).Control(7)=   "optDelAcc"
         Tab(0).Control(7).Enabled=   0   'False
         Tab(0).Control(8)=   "optAddAcc"
         Tab(0).Control(8).Enabled=   0   'False
         Tab(0).Control(9)=   "optListAcc"
         Tab(0).Control(9).Enabled=   0   'False
         Tab(0).Control(10)=   "optAccessLvl"
         Tab(0).Control(10).Enabled=   0   'False
         Tab(0).ControlCount=   11
         TabCaption(1)   =   "S&et Commands"
         TabPicture(1)   =   "frmMain.frx":055A
         Tab(1).ControlEnabled=   0   'False
         Tab(1).Control(0)=   "lblKill"
         Tab(1).Control(1)=   "lblNoMemo"
         Tab(1).Control(2)=   "lblNOOP"
         Tab(1).Control(3)=   "lblShowEmail"
         Tab(1).Control(4)=   "optSetPass"
         Tab(1).Control(5)=   "optSetURL"
         Tab(1).Control(6)=   "optEmail"
         Tab(1).Control(7)=   "optKillOn"
         Tab(1).Control(8)=   "optKillOff"
         Tab(1).Control(9)=   "optMemoOn"
         Tab(1).Control(10)=   "optMemoOff"
         Tab(1).Control(11)=   "optNOOPOn"
         Tab(1).Control(12)=   "optNOOPOff"
         Tab(1).Control(13)=   "optShowEmailOn"
         Tab(1).Control(14)=   "optShowEmailOff"
         Tab(1).ControlCount=   15
         Begin VB.OptionButton optShowEmailOff 
            Caption         =   "OFF"
            Height          =   255
            Left            =   -70680
            TabIndex        =   35
            Top             =   1560
            Width           =   615
         End
         Begin VB.OptionButton optShowEmailOn 
            Caption         =   "ON"
            Height          =   255
            Left            =   -71400
            TabIndex        =   34
            Top             =   1560
            Width           =   615
         End
         Begin VB.OptionButton optNOOPOff 
            Caption         =   "OFF"
            Height          =   255
            Left            =   -70680
            TabIndex        =   33
            Top             =   1320
            Width           =   615
         End
         Begin VB.OptionButton optNOOPOn 
            Caption         =   "ON"
            Height          =   255
            Left            =   -71400
            TabIndex        =   32
            Top             =   1320
            Width           =   615
         End
         Begin VB.OptionButton optMemoOff 
            Caption         =   "OFF"
            Height          =   255
            Left            =   -70680
            TabIndex        =   31
            Top             =   1080
            Width           =   615
         End
         Begin VB.OptionButton optMemoOn 
            Caption         =   "ON"
            Height          =   255
            Left            =   -71400
            TabIndex        =   30
            Top             =   1080
            Width           =   615
         End
         Begin VB.OptionButton optKillOff 
            Caption         =   "OFF"
            Height          =   255
            Left            =   -70680
            TabIndex        =   29
            Top             =   840
            Width           =   615
         End
         Begin VB.OptionButton optKillOn 
            Caption         =   "ON"
            Height          =   255
            Left            =   -71400
            TabIndex        =   28
            Top             =   840
            Width           =   615
         End
         Begin VB.OptionButton optEmail 
            Caption         =   "Set Email for Nickname"
            Height          =   255
            Left            =   -72960
            TabIndex        =   23
            Top             =   480
            Width           =   2055
         End
         Begin VB.OptionButton optSetURL 
            Caption         =   "Set URL for Nickname"
            Height          =   255
            Left            =   -70800
            TabIndex        =   22
            Top             =   480
            Width           =   2055
         End
         Begin VB.OptionButton optSetPass 
            Caption         =   "Set New Password"
            Height          =   255
            Left            =   -74760
            TabIndex        =   21
            Top             =   480
            Width           =   1695
         End
         Begin VB.OptionButton optAccessLvl 
            Caption         =   "Current Access Level"
            Height          =   255
            Left            =   3120
            TabIndex        =   20
            Top             =   600
            Width           =   2535
         End
         Begin VB.OptionButton optListAcc 
            Caption         =   "List Access Level Masks"
            Height          =   255
            Left            =   3120
            TabIndex        =   19
            Top             =   840
            Width           =   2535
         End
         Begin VB.OptionButton optAddAcc 
            Caption         =   "Add Access Mask"
            Height          =   255
            Left            =   3120
            TabIndex        =   18
            Top             =   1080
            Width           =   2535
         End
         Begin VB.OptionButton optDelAcc 
            Caption         =   "Delete Access Mask"
            Height          =   255
            Left            =   3120
            TabIndex        =   17
            Top             =   1320
            Width           =   2535
         End
         Begin VB.OptionButton optDrop 
            Caption         =   "Drop Nickname"
            Height          =   255
            Left            =   240
            TabIndex        =   16
            Top             =   1320
            Width           =   1935
         End
         Begin VB.OptionButton optInfo 
            Caption         =   "Nickname Information"
            Height          =   255
            Left            =   3120
            TabIndex        =   15
            Top             =   360
            Width           =   1935
         End
         Begin VB.OptionButton optRelease 
            Caption         =   "Release Nickname"
            Height          =   255
            Left            =   240
            TabIndex        =   14
            Top             =   1560
            Width           =   1935
         End
         Begin VB.OptionButton optGhost 
            Caption         =   "Ghost Nickname"
            Height          =   255
            Left            =   240
            TabIndex        =   13
            Top             =   1080
            Width           =   1935
         End
         Begin VB.OptionButton optRecover 
            Caption         =   "Recover Nickname"
            Height          =   255
            Left            =   240
            TabIndex        =   12
            Top             =   840
            Width           =   1935
         End
         Begin VB.OptionButton optIdentify 
            Caption         =   "Identify to Nickserv"
            Height          =   255
            Left            =   240
            TabIndex        =   11
            Top             =   600
            Width           =   1935
         End
         Begin VB.OptionButton optRegister 
            Caption         =   "Register Nickname"
            Height          =   255
            Left            =   240
            TabIndex        =   5
            Top             =   360
            Width           =   1935
         End
         Begin VB.Label lblShowEmail 
            Alignment       =   1  'Right Justify
            Caption         =   "Show Associated Email"
            Height          =   255
            Left            =   -73680
            TabIndex        =   27
            Top             =   1560
            Width           =   2175
         End
         Begin VB.Label lblNOOP 
            Alignment       =   1  'Right Justify
            Caption         =   "Do not allow Nick to be OP'd"
            Height          =   255
            Left            =   -73680
            TabIndex        =   26
            Top             =   1320
            Width           =   2175
         End
         Begin VB.Label lblNoMemo 
            Alignment       =   1  'Right Justify
            Caption         =   "Turn off Memo functions"
            Height          =   255
            Left            =   -73680
            TabIndex        =   25
            Top             =   1080
            Width           =   2175
         End
         Begin VB.Label lblKill 
            Alignment       =   1  'Right Justify
            Caption         =   "Kill unidentified users with Nick"
            Height          =   255
            Left            =   -73680
            TabIndex        =   24
            Top             =   840
            Width           =   2175
         End
      End
      Begin VB.Label lblPassword 
         Caption         =   "Password"
         Height          =   255
         Left            =   -72480
         TabIndex        =   49
         Top             =   840
         Width           =   735
      End
      Begin VB.Label lblChannel 
         Caption         =   "Channel"
         Height          =   255
         Left            =   -72480
         TabIndex        =   48
         Top             =   1200
         Width           =   735
      End
      Begin VB.Label lblNickname 
         Caption         =   "Nickname"
         Height          =   255
         Left            =   -72480
         TabIndex        =   47
         Top             =   480
         Width           =   735
      End
   End
   Begin VB.CommandButton cmdSend 
      Caption         =   "&Send"
      Default         =   -1  'True
      Height          =   375
      Left            =   4440
      TabIndex        =   8
      Top             =   3960
      Width           =   1215
   End
   Begin VB.Label lblInput 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      BorderStyle     =   1  'Fixed Single
      Caption         =   "Misc User Input"
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   120
      TabIndex        =   51
      Top             =   2760
      Width           =   1815
   End
   Begin VB.Label lblCommand 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      BorderStyle     =   1  'Fixed Single
      Caption         =   "Command to be sent"
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   120
      TabIndex        =   50
      Top             =   3360
      Width           =   1815
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Load()
    DrawGradient Me, 0, 0, 255, 0, 1, 0, Me.ScaleHeight, 1, True, 1, 1, 1
End Sub

Private Sub cmdGetSettings_Click()
    txtNickname.Text = GetSetting("DalNET Services Command Center", "Settings", "Nickname")
    txtPassword.Text = GetSetting("DalNET Services Command Center", "Settings", "Password")
    txtChannel.Text = GetSetting("DalNET Services Command Center", "Settings", "Channel")
    chkTransparent.Value = GetSetting("DalNET Services Command Center", "Settings", "Transparent")
End Sub

Private Sub cmdSave_Click()
    SaveSetting "DalNET Services Command Center", "Settings", "Nickname", txtNickname.Text
    SaveSetting "DalNET Services Command Center", "Settings", "Password", txtPassword.Text
    SaveSetting "DalNET Services Command Center", "Settings", "Channel", txtChannel.Text
    SaveSetting "DalNET Services Command Center", "Settings", "Transparent", chkTransparent.Value
End Sub

Private Sub cmdDelSettings_Click()
    Dim Answer As String
    Answer = MsgBox("Are you sure?", vbQuestion + vbYesNo, "Are you sure?")
    If Answer = 6 Then
        DeleteSetting "DalNET Services Command Center", "Settings", "Nickname"
        DeleteSetting "DalNET Services Command Center", "Settings", "Password"
        DeleteSetting "DalNET Services Command Center", "Settings", "Channel"
    Else
        Exit Sub
    End If
End Sub

Private Sub cmdSend_Click()
    If txtOutput <> "" Then
        txtOutput.LinkMode = vbLinkNone
        frmMain.txtOutput.LinkItem = txtOutput.Text
        txtOutput.LinkMode = vbLinkManual
        txtOutput.LinkPoke
        txtOutput.Text = ""
    Else
        MsgBox "Please select a command to run!", vbCritical + vbOKOnly, "Error - Input Required!"
    End If
End Sub

Private Sub cmdExit_Click()
    Unload Me
End Sub

Private Sub chkTransparent_Click()
    Dim lngVersion As Long
    lngVersion = GetVersion()
    If lngVersion = 143851525 Then
        If chkTransparent.Value = 1 Then
            SetLayered Me.hWnd, True, 175
        Else
            SetLayered Me.hWnd, False, 0
        End If
    Else
        MsgBox "Sorry, this only works on Windows 2000 machines.", vbInformation + vbOKOnly, "Invalid Function"
    End If
End Sub

Private Sub optRegister_Click()
    Dim Password As String, Email As String
    Password = txtPassword.Text
    Email = txtPassword.Text
    If Password = "" Then
        Password = InputBox("Please enter your Password!", "Input Required!")
    End If
    If Email = "" Then
        Email = InputBox("Please enter your email address!", "Input Required!")
    End If
    txtOutput.Text = "/msg nickserv@services.dal.net register " & Password & " " & Email
End Sub

Private Sub optIdentify_Click()
    Dim Password As String
    Password = txtPassword.Text
    If Password = "" Then
        Password = InputBox("Please enter your Password!", "Input Required!")
    End If
    txtOutput.Text = "/msg nickserv@services.dal.net identify " & Password
End Sub

Private Sub optRecover_Click()
    Dim Nickname As String, Password As String
    Nickname = txtNickname.Text
    Password = txtPassword.Text
    If Nickname = "" Then
        Nickname = InputBox("Please enter your Nickname!", "Input Required!")
    End If
    If Password = "" Then
        Password = InputBox("Please enter your Password!", "Input Required!")
    End If
    txtOutput.Text = "/msg nickserv@services.dal.net recover " & Nickname & " " & Password
End Sub

Private Sub optGhost_Click()
    Dim Nickname As String, Password As String
    Nickname = txtNickname.Text
    Password = txtPassword.Text
    If Password = "" Then
        Password = InputBox("Please enter your Password!", "Input Required!")
    End If
    If Nickname = "" Then
        Nickname = InputBox("Please enter your Nickname!", "Input Required!")
    End If
    txtOutput.Text = "/msg nickserv@services.dal.net ghost " & Nickname & " " & Password
End Sub

Private Sub optDrop_Click()
    Dim Nickname As String
    Nickname = txtNickname.Text
    If Nickname = "" Then
        Nickname = InputBox("Please enter your Nickname!", "Input Required!")
    End If
    txtOutput.Text = "/msg nickserv@services.dal.net drop " & Nickname
End Sub

Private Sub optRelease_Click()
    txtOutput.Text = "/msg nickserv@services.dal.net release " & txtNickname.Text & " " & txtPassword.Text
End Sub

Private Sub optInfo_Click()
    Dim Nickname As String
    Nickname = txtNickname.Text
    If Nickname = "" Then
        Nickname = InputBox("Please enter your Nickname!", "Input Required!")
    End If
    txtOutput.Text = "/msg nickserv@services.dal.net info " & Nickname
End Sub

Private Sub optAccessLvl_Click()
    Dim Nickname As String
    Nickname = txtNickname.Text
    If Nickname = "" Then
        Nickname = InputBox("Please enter your Nickname!", "Input Required!")
    End If
    txtOutput.Text = "/msg nickserv@services.dal.net acc " & Nickname
End Sub

Private Sub optListAcc_Click()
    txtOutput.Text = "/msg nickserv@services.dal.net access list"
End Sub

Private Sub optAddAcc_Click()
    Dim Memotext As String
    Memotext = txtMemo.Text
    If Memotext = "" Then
        Memotext = InputBox("Please enter Address Mask!", "Input Required!")
    End If
    txtOutput.Text = "/msg nickserv@services.dal.net access add " & Memotext
End Sub

Private Sub optDelAcc_Click()
    Dim Memotext As String
    Memotext = txtMemo.Text
    If Memotext = "" Then
        Memotext = InputBox("Please enter Address Mask!", "Input Required!")
    End If
    txtOutput.Text = "/msg nickserv@services.dal.net access del " & Memotext
End Sub

Private Sub optSetPass_Click()
    Dim Password As String
    Password = txtPassword.Text
    If Password = "" Then
        Password = InputBox("Please enter your new Password!", "Input Required!")
    End If
    txtOutput.Text = "/msg nickserv@services.dal.net set passwd " & Password
End Sub

Private Sub optSetURL_Click()
    Dim Memotext As String
    Memotext = txtMemo.Text
    If Memotext = "" Then
        Memotext = InputBox("Please enter the URL to associate with the Nickname!", "Input Required!")
    End If
    txtOutput.Text = "/msg nickserv@services.dal.net set url " & Memotext
End Sub

Private Sub optEmail_Click()
    Dim Memotext As String
    Memotext = txtMemo.Text
    If Memotext = "" Then
        Memotext = InputBox("Please enter your email address!", "Input Required!")
    End If
    txtOutput.Text = "/msg nickserv@services.dal.net set email " & Memotext
End Sub

Private Sub optKillOff_Click()
    txtOutput.Text = "/msg nickserv@services.dal.net set kill off"
End Sub

Private Sub optKillOn_Click()
    txtOutput.Text = "/msg nickserv@services.dal.net set kill on"
End Sub

Private Sub optMemoOff_Click()
    txtOutput.Text = "/msg nickserv@services.dal.net set nomemo off"
End Sub

Private Sub optMemoOn_Click()
    txtOutput.Text = "/msg nickserv@services.dal.net set nomemo on"
End Sub

Private Sub optNOOPOff_Click()
    txtOutput.Text = "/msg nickserv@services.dal.net set noop off"
End Sub

Private Sub optNOOPOn_Click()
    txtOutput.Text = "/msg nickserv@services.dal.net set noop on"
End Sub

Private Sub optShowEmailOff_Click()
    txtOutput.Text = "/msg nickserv@services.dal.net set showemail off"
End Sub

Private Sub optShowEmailOn_Click()
    txtOutput.Text = "/msg nickserv@services.dal.net set showemail on"
End Sub

Private Sub optList_Click()
    txtOutput.Text = "/msg memoserv@services.dal.net list"
End Sub

Private Sub optRead_Click()
    Dim MessageNo As String
    MessageNo = txtMemo.Text
    If MessageNo = "" Then
        MessageNo = InputBox("Please enter the message number to undelete", "Input required...")
    End If
    txtOutput.Text = "/msg memoserv@services.dal.net read " & MessageNo
End Sub

Private Sub optSend_Click()
    Dim Memo As String, Nickname As String
    Memo = txtMemo.Text
    Nickname = txtNickname.Text
    If Memo = "" Then
        Memo = InputBox("Please enter the message to send", "Input required...")
    End If
    If Nickname = "" Then
        Nickname = InputBox("Please enter the Nickname to send the message to", "Input required...")
    End If
    txtOutput.Text = "/msg memoserv@services.dal.net send " & Nickname & " " & Memo
End Sub

Private Sub optSendSOP_Click()
    Dim Memo As String, Channel As String
    Memo = txtMemo.Text
    Channel = txtChannel.Text
    If Memo = "" Then
        Memo = InputBox("Please enter the message to send", "Input required...")
    End If
    If Channel = "" Then
        Channel = InputBox("Please enter the channel to send the message to", "Input required...")
    End If
    txtOutput.Text = "/msg memoserv@services.dal.net sendsop " & Channel & " " & Memo
End Sub

Private Sub optDelete_Click()
    Dim MessageNo As String
    MessageNo = txtMemo.Text
    If MessageNo = "" Then
        MessageNo = InputBox("Please enter the message number to undelete", "Input required...")
    End If
    txtOutput.Text = "/msg memoserv@services.dal.net del " & MessageNo
End Sub

Private Sub optUnDelete_Click()
    Dim MessageNo As String
    MessageNo = txtMemo.Text
    If MessageNo = "" Then
        MessageNo = InputBox("Please enter the message number to undelete", "Input required...")
    End If
    txtOutput.Text = "/msg memoserv@services.dal.net undel " & MessageNo
End Sub

Private Sub optPurge_Click()
    txtOutput.Text = "/msg memoserv@services.dal.net purge"
End Sub

Private Sub optFwdStatus_Click()
    txtOutput.Text = "/msg memoserv@services.dal.net forward"
End Sub

Private Sub optFwdOn_Click()
    Dim Nickname As String, Password As String
    Nickname = txtNickname.Text
    Password = txtPassword.Text
    If Nickname = "" Then
        Nickname = InputBox("Please enter your Nickname!", "Input Required!")
    End If
    If Password = "" Then
        Password = InputBox("Please enter your Password!", "Input Required!")
    End If
    txtOutput.Text = "/msg memoserv@services.dal.net forward " & Nickname & " " & Password
End Sub

Private Sub optFwdOff_Click()
    txtOutput.Text = "/msg memoserv@services.dal.net forward -"
End Sub

Private Sub optChanReg_Click()
    Dim Password As String, Channel As String, Desc As String
    Channel = txtChannel.Text
    Password = txtPassword.Text
    Desc = txtMemo.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Password = "" Then
        Password = InputBox("Please enter your Password!", "Input Required!")
    End If
    If Desc = "" Then
        Desc = InputBox("Please enter a channel description!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net register " & Channel & " " & Password & " " & Desc
End Sub

Private Sub optChanDrop_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net drop " & Channel
End Sub

Private Sub optChanID_Click()
    Dim Password As String, Channel As String
    Channel = txtChannel.Text
    Password = txtPassword.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If txtPassword.Text = "" Then
        Password = InputBox("Please enter your Password!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net identify " & Channel & " " & Password
End Sub

Private Sub optChanAccess_Click()
    Dim Channel As String, Nickname As String
    Channel = txtChannel.Text
    Nickname = txtNickname.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Nickname = "" Then
        Nickname = InputBox("Please enter the Nickname!", "Input Optional!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net access " & Channel & " " & Nickname
End Sub

Private Sub optChanCount_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net count " & Channel
End Sub

Private Sub optChanWhy_Click()
    Dim Channel As String, Nickname As String
    Channel = txtChannel.Text
    Nickname = txtNickname.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Nickname = "" Then
        Nickname = InputBox("Please enter the Nickname!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net why " & Channel & " " & Nickname
End Sub

Private Sub optChanInfo_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net info " & Channel
End Sub

Private Sub optChanInvite_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net invite " & Channel
End Sub

Private Sub optChanMKick_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net mkick " & Channel
End Sub

Private Sub optChanMDEOP_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net mdeop " & Channel
End Sub

Private Sub optChanDeop_Click()
    Dim Channel As String, Nickname As String
    Channel = txtChannel.Text
    Nickname = txtNickname.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Nickname = "" Then
        Nickname = InputBox("Please enter the Nickname!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net deop " & Channel & " " & Nickname
End Sub

Private Sub optChanUnBan_Click()
    Dim Channel As String, Memo As String
    Channel = txtChannel.Text
    Memo = txtMemo.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Memo = "" Then
        Memo = InputBox("Please enter ME for AOPs or ALL for SOPs!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net why " & Channel & " " & Memo
End Sub

Private Sub optChanOpUser_Click()
    Dim Channel As String, Nickname As String
    Channel = txtChannel.Text
    Nickname = txtNickname.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Nickname = "" Then
        Nickname = InputBox("Please enter the Nickname!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net op " & Channel & " " & Nickname
End Sub

Private Sub optChanDeopUser_Click()
    Dim Channel As String, Nickname As String
    Channel = txtChannel.Text
    Nickname = txtNickname.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Nickname = "" Then
        Nickname = InputBox("Please enter the Nickname!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net deop " & Channel & " " & Nickname
End Sub

Private Sub optChanSOPList_Click()
    Dim Channel As String, Memo As String
    Channel = txtChannel.Text
    Memo = txtMemo.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If txtMemo.Text = "" Then
        Memo = InputBox("Please enter a search pattern!", "Input Optional!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net sop list " & Channel & " " & Memo
End Sub

Private Sub optChanSOPAdd_Click()
    Dim Channel As String, Nickname As String
    Channel = txtChannel.Text
    Nickname = txtNickname.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Nickname = "" Then
        Nickname = InputBox("Please enter the Nickname or Mask!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net sop add " & Channel & " " & Nickname
End Sub

Private Sub optChanSOPDel_Click()
    Dim Channel As String, Nickname As String
    Channel = txtChannel.Text
    Nickname = txtNickname.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Nickname = "" Then
        Nickname = InputBox("Please enter the Index Number or Mask!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net sop del " & Channel & " " & Nickname
End Sub

Private Sub optChanSOPWipe_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net sop wipe " & Channel
End Sub

Private Sub optChanSOPClean_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net sop clean " & Channel
End Sub

Private Sub optChanAOPList_Click()
    Dim Channel As String, Memo As String
    Channel = txtChannel.Text
    Memo = txtMemo.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Memo = "" Then
        Memo = InputBox("Please enter a search pattern!", "Input Optional!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net aop list " & Channel & " " & Memo
End Sub

Private Sub optChanAOPAdd_Click()
    Dim Channel As String, Nickname As String
    Channel = txtChannel.Text
    Nickname = txtNickname.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Nickname = "" Then
        Nickname = InputBox("Please enter the Nickname or Mask!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net aop add " & Channel & " " & Nickname
End Sub

Private Sub optChanAOPDel_Click()
    Dim Channel As String, Nickname As String
    Channel = txtChannel.Text
    Nickname = txtNickname.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Nickname = "" Then
        Nickname = InputBox("Please enter the Nickname or Mask!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net aop del " & Channel & " " & Nickname
End Sub

Private Sub optChanAOPWipe_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net aop wipe " & Channel
End Sub

Private Sub optChanAOPClean_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net aop clean " & Channel
End Sub

Private Sub optChanAKickList_Click()
    Dim Channel As String, Memo As String
    Channel = txtChannel.Text
    Memo = txtMemo.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Memo = "" Then
        Memo = InputBox("Please enter a search pattern!", "Input Optional!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net akick list " & Channel & " " & Memo
End Sub

Private Sub optChanAKickAdd_Click()
    Dim Channel As String, Nickname As String
    Channel = txtChannel.Text
    Nickname = txtNickname.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Nickname = "" Then
        Nickname = InputBox("Please enter the Nickname or Mask!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net akick add " & Channel & " " & Nickname
End Sub

Private Sub optChanAKickDel_Click()
    Dim Channel As String, Nickname As String
    Channel = txtChannel.Text
    Nickname = txtNickname.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Nickname = "" Then
        Nickname = InputBox("Please enter the Nickname or Mask!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net akick del " & Channel & " " & Nickname
End Sub

Private Sub optChanAKickWipe_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net akick wipe " & Channel
End Sub

Private Sub optSetChanFounder_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " founder"
End Sub

Private Sub optSetChanPass_Click()
    Dim Channel As String, Password As String
    Channel = txtChannel.Text
    Password = txtPassword.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Password = "" Then
        Password = InputBox("Please enter the new Password!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " passwd " & Password
End Sub

Private Sub optSetChanDesc_Click()
    Dim Channel As String, Desc As String
    Channel = txtChannel.Text
    Desc = txtMemo.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Desc = "" Then
        Desc = InputBox("Please enter the new Description!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " desc " & Desc
End Sub

Private Sub optSetChanMode_Click()
    Dim Channel As String, Modes As String
    Channel = txtChannel.Text
    Modes = txtMemo.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If Modes = "" Then
        Modes = InputBox("Please enter the Channel Modes! (ex. +tn-ispklm)", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " mlock " & Modes
End Sub

Private Sub optSetChanURL_Click()
    Dim Channel As String, URL As String
    Channel = txtChannel.Text
    URL = txtMemo.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    If URL = "" Then
        URL = InputBox("Please enter the Channel URL!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " url " & URL
End Sub

Private Sub optSetChanTLFounder_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " topiclock founder"
End Sub

Private Sub optSetChanTLSOP_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " topiclock sop"
End Sub

Private Sub optSetChanTLAOP_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " topiclock aop"
End Sub

Private Sub optSetChanMemoFounder_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " memo founder"
End Sub

Private Sub optSetChanMemoSOP_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " memo sop"
End Sub

Private Sub optSetChanMemoAOP_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " memo aop"
End Sub

Private Sub optSetChanMemoNone_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " memo none"
End Sub

Private Sub optSetChanOpGrdON_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " opguard on"
End Sub

Private Sub optSetChanOpGrdOFF_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " opguard off"
End Sub

Private Sub optSetChanKTON_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " keeptopic on"
End Sub

Private Sub optSetChanKTOFF_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " keeptopic off"
End Sub

Private Sub optSetChanOPIDON_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " identify on"
End Sub

Private Sub optSetChanOPIDOFF_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " identify off"
End Sub

Private Sub optSetChanRestON_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " restrict on"
End Sub

Private Sub optSetChanRestOFF_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " restrict off"
End Sub

Private Sub optSetChanLopON_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " leaveops on"
End Sub

Private Sub optSetChanLopOFF_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " leaveops off"
End Sub

Private Sub optSetChanUnSecON_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " unsecure on"
End Sub

Private Sub optSetChanUnSecOFF_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " unsecure off"
End Sub

Private Sub optSetChanPvtON_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " private on"
End Sub

Private Sub optSetChanPvtOFF_Click()
    Dim Channel As String
    Channel = txtChannel.Text
    If Channel = "" Then
        Channel = InputBox("Please enter the Channel Name!", "Input Required!")
    End If
    txtOutput.Text = "/msg chanserv@services.dal.net set " & Channel & " private off"
End Sub

