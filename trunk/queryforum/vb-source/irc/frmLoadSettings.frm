VERSION 5.00
Begin VB.Form LoadSettings 
   BorderStyle     =   3  'Fixed Dialog
   ClientHeight    =   540
   ClientLeft      =   7605
   ClientTop       =   5055
   ClientWidth     =   3495
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   540
   ScaleWidth      =   3495
   ShowInTaskbar   =   0   'False
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "Loading Settings, Please wait ..."
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   270
      Left            =   150
      TabIndex        =   0
      Top             =   150
      Width           =   3300
   End
End
Attribute VB_Name = "LoadSettings"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
