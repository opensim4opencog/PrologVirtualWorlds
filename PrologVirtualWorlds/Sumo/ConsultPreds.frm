VERSION 5.00
Begin VB.Form ConsultPreds 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "External Predicate"
   ClientHeight    =   1305
   ClientLeft      =   8820
   ClientTop       =   8730
   ClientWidth     =   4650
   Icon            =   "ConsultPreds.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1305
   ScaleWidth      =   4650
   Begin VB.ComboBox Predicate 
      Height          =   315
      Left            =   105
      Style           =   2  'Dropdown List
      TabIndex        =   11
      Top             =   60
      Width           =   3240
   End
   Begin VB.CommandButton Command4 
      Caption         =   "OK"
      Height          =   330
      Left            =   2550
      TabIndex        =   10
      Top             =   900
      Width           =   915
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Edit"
      Height          =   330
      Left            =   105
      TabIndex        =   9
      Top             =   900
      Width           =   825
   End
   Begin VB.CommandButton Command2 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   345
      Left            =   3555
      TabIndex        =   8
      Top             =   885
      Width           =   1005
   End
   Begin VB.CommandButton Command1 
      Caption         =   "New"
      Height          =   330
      Left            =   990
      TabIndex        =   7
      Top             =   900
      Width           =   915
   End
   Begin VB.ComboBox Often 
      Height          =   315
      ItemData        =   "ConsultPreds.frx":0442
      Left            =   3060
      List            =   "ConsultPreds.frx":044F
      Style           =   2  'Dropdown List
      TabIndex        =   6
      Top             =   450
      Width           =   1515
   End
   Begin VB.OptionButton Location 
      Caption         =   "Consultation"
      Height          =   285
      Index           =   1
      Left            =   1770
      TabIndex        =   4
      Top             =   480
      Width           =   1320
   End
   Begin VB.OptionButton Location 
      Caption         =   "Module"
      Height          =   285
      Index           =   2
      Left            =   930
      TabIndex        =   5
      Top             =   480
      Width           =   960
   End
   Begin VB.OptionButton Location 
      Caption         =   "Inactive"
      Height          =   285
      Index           =   0
      Left            =   30
      TabIndex        =   3
      Top             =   480
      Value           =   -1  'True
      Width           =   900
   End
   Begin VB.ComboBox ArgMode 
      BeginProperty Font 
         Name            =   "Verdana"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   360
      Index           =   0
      ItemData        =   "ConsultPreds.frx":0471
      Left            =   3330
      List            =   "ConsultPreds.frx":047E
      TabIndex        =   2
      Text            =   "<mode>"
      Top             =   1440
      Width           =   1230
   End
   Begin VB.ComboBox ArgDomain 
      BeginProperty Font 
         Name            =   "Verdana"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   360
      Index           =   0
      Left            =   1005
      TabIndex        =   1
      Text            =   "<domain>"
      Top             =   1575
      Width           =   2115
   End
   Begin VB.ComboBox Arity 
      BeginProperty Font 
         Name            =   "Microsoft Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      ItemData        =   "ConsultPreds.frx":049F
      Left            =   3465
      List            =   "ConsultPreds.frx":04BB
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   45
      Width           =   1155
   End
End
Attribute VB_Name = "ConsultPreds"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
