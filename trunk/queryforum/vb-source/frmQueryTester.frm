VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Begin VB.Form frmUserModel 
   BackColor       =   &H80000000&
   Caption         =   "Structured Query Object Container"
   ClientHeight    =   3975
   ClientLeft      =   5760
   ClientTop       =   5550
   ClientWidth     =   7275
   BeginProperty Font 
      Name            =   "Arial"
      Size            =   9.75
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "frmQueryTester.frx":0000
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   3975
   ScaleWidth      =   7275
   Begin VB.ComboBox txtMessage 
      Height          =   360
      Left            =   3360
      TabIndex        =   11
      Text            =   "txtMessage"
      Top             =   2880
      Width           =   3135
   End
   Begin VB.CommandButton cmdXMIT 
      Caption         =   "XMIT"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   11.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   5640
      TabIndex        =   10
      Top             =   3240
      Width           =   855
   End
   Begin VB.ComboBox TargetObject 
      Height          =   360
      Left            =   0
      TabIndex        =   9
      Text            =   "TargetObject"
      Top             =   2880
      Width           =   1695
   End
   Begin VB.ComboBox txtSubscriptions 
      Height          =   360
      Left            =   1800
      TabIndex        =   8
      Text            =   "Subscriptions"
      Top             =   3240
      Width           =   1455
   End
   Begin VB.ComboBox txtNativeLanguage 
      Height          =   360
      ItemData        =   "frmQueryTester.frx":030A
      Left            =   3360
      List            =   "frmQueryTester.frx":030C
      TabIndex        =   7
      Text            =   "NativeLanguage"
      Top             =   3240
      Width           =   1575
   End
   Begin VB.TextBox txtFriendlyName 
      Height          =   285
      Left            =   1800
      TabIndex        =   6
      Text            =   "FriendlyName"
      Top             =   2880
      Width           =   1455
   End
   Begin VB.ComboBox txtDistributions 
      Height          =   360
      Left            =   0
      TabIndex        =   5
      Text            =   "Distributions"
      Top             =   3240
      Width           =   1695
   End
   Begin MSComctlLib.StatusBar sbrDB 
      Align           =   2  'Align Bottom
      Height          =   255
      Left            =   0
      TabIndex        =   0
      Top             =   3720
      Width           =   7275
      _ExtentX        =   12832
      _ExtentY        =   450
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   1
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
         EndProperty
      EndProperty
   End
   Begin RichTextLib.RichTextBox OutputWindow 
      Height          =   2415
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   6495
      _ExtentX        =   11456
      _ExtentY        =   4260
      _Version        =   393217
      ScrollBars      =   3
      TextRTF         =   $"frmQueryTester.frx":030E
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
   Begin VB.Label Label1 
      BackColor       =   &H80000000&
      Caption         =   "To:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   11.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   0
      Left            =   0
      TabIndex        =   4
      Top             =   2520
      Width           =   1335
   End
   Begin VB.Label Label1 
      BackColor       =   &H80000000&
      Caption         =   "Message:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   11.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   2
      Left            =   3360
      TabIndex        =   3
      Top             =   2520
      Width           =   1335
   End
   Begin VB.Label Label1 
      BackColor       =   &H80000000&
      Caption         =   "From:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   11.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   1
      Left            =   1920
      TabIndex        =   2
      Top             =   2520
      Width           =   855
   End
   Begin VB.Menu mnuFile 
      Caption         =   "File"
      WindowList      =   -1  'True
   End
   Begin VB.Menu mnu 
      Caption         =   "Edit"
   End
   Begin VB.Menu mnuView 
      Caption         =   "View"
   End
   Begin VB.Menu mnuProlog 
      Caption         =   "Prolog"
      Begin VB.Menu cmdPROQUERYconnect 
         Caption         =   "Connect"
      End
      Begin VB.Menu cmdProqueryDisconnect 
         Caption         =   "Disconnect"
      End
   End
   Begin VB.Menu mnuSQL 
      Caption         =   "SQL"
      Begin VB.Menu cmdSQLConnect 
         Caption         =   "Connect"
      End
      Begin VB.Menu cmdSQLDisconnect 
         Caption         =   "Disconnect"
      End
   End
   Begin VB.Menu mnuMOO 
      Caption         =   "MOO"
      Begin VB.Menu cmdMOOConnect 
         Caption         =   "Connect"
      End
      Begin VB.Menu cmdMOODisconnect 
         Caption         =   "Disconnect"
      End
   End
   Begin VB.Menu mnuIRC 
      Caption         =   "IRC"
      Begin VB.Menu cmdIRCConnect 
         Caption         =   "Connect"
      End
   End
   Begin VB.Menu mnuNlpWin 
      Caption         =   "NlpWin"
      Begin VB.Menu cmdNlpWinConnect 
         Caption         =   "Connect"
      End
   End
   Begin VB.Menu mnuRAID 
      Caption         =   "RAID"
      Begin VB.Menu cmdRaidconnect 
         Caption         =   "Connect"
      End
   End
   Begin VB.Menu mnuSscriptControl 
      Caption         =   "ScriptControl"
      Begin VB.Menu cmdScriptConnect 
         Caption         =   "Connect"
      End
      Begin VB.Menu cmdVBScript 
         Caption         =   "VBScript"
      End
      Begin VB.Menu cmdJScript 
         Caption         =   "JScript"
      End
   End
   Begin VB.Menu mnuRawSocket 
      Caption         =   "RawSocket"
      Begin VB.Menu cmdRawSocketConnect 
         Caption         =   "Connect"
      End
   End
   Begin VB.Menu mnuMSBN 
      Caption         =   "MSBN"
      Begin VB.Menu cmdMSBNConnect 
         Caption         =   "Connect"
      End
   End
End
Attribute VB_Name = "frmUserModel"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private mNode As Node ' Module-level Node variable.
Private mItem As ListItem ' Module-level ListItem variable.
Private EventFlag As Integer ' To signal which event has occurred.
Private mCurrentIndex As Integer ' Flag to assure this node wasn't already clicked.
Private mStatusBarStyle As Integer ' Switches Statusbar style.

Private Sub Form_Load()
    Dim Item As Long
        
        Exit Sub
        WriteToUser "system", "Log started on " & VBA.Date & " at " & VBA.Time & vbCrLf
        
        'NewRegistrant "everyone"
        
        'Item = NewRegistrant("theMoo")
        'TestObj(Item).NativeLanguage = "moo"
        'TestObj(Item).FriendlyName = "theMoo"
        'Void TestObj(Item).Consult("lingua.utdallas.edu:8888:nephrael:moomoo")
        
       ' Item = NewRegistrant("prolog")
       ' TestObj(Item).NativeLanguage = "prolog"
       ' TestObj(Item).FriendlyName = "prolog"
        
       ' Item = NewRegistrant("theSystem")
        
       ' Item = NewRegistrant("theUser")
        
       ' Item = NewRegistrant("SQLQuery")
       ' TestObj(Item).NativeLanguage = "sql"
       ' Void TestObj(Item).Consult("{sql server}:MMLAB:vct::inventory")
       '
       ' Item = NewRegistrant("VBScriptQuery")
       ' TestObj(Item).NativeLanguage = "vbscript"
       '
        'Item = NewRegistrant("JScriptQuery")
        'TestObj(Item).NativeLanguage = "jscript"
        
       ' Item = NewRegistrant("XMLQuery")
       ' TestObj(Item).NativeLanguage = "xml"
        
        'Item = NewRegistrant("RAIDQuery")
        'TestObj(Item).NativeLanguage = "raid"
        
        'Item = NewRegistrant("NLPQuery")
        'TestObj(Item).NativeLanguage = "english"

   
End Sub

Private Sub form_Resize()
    On Error Resume Next
    Dim i As Long
    
    Debug.Print "Resize: " & Width & ", " & Height
    OutputWindow.Width = Me.Width
    OutputWindow.Left = 0
    OutputWindow.Top = 0
    
    OutputWindow.Height = Int(Me.ScaleHeight - txtNativeLanguage.Height - txtMessage.Height - Label1(0).Height)
    Label1(0).Top = OutputWindow.Height + 20
    Label1(1).Top = OutputWindow.Height + 20
    Label1(2).Top = OutputWindow.Height + 20
    Label1(0).Left = 0
    Label1(1).Left = Int(Me.Width * 0.25)
    Label1(2).Left = Int(Me.Width * 0.5)
    Label1(0).Width = Int(Me.Width * 0.25)
    Label1(1).Width = Int(Me.Width * 0.25)
    Label1(2).Width = Int(Me.Width * 0.25)
    TargetObject.Left = Label1(0).Left
    txtFriendlyName.Left = Label1(1).Left
    txtMessage.Left = Label1(2).Left
    TargetObject.Width = Label1(0).Width
    txtFriendlyName.Width = Label1(1).Width
    txtMessage.Width = Label1(2).Width * 2
    i = Label1(0).Height + OutputWindow.Height
    TargetObject.Top = i
    txtFriendlyName.Top = i
    txtMessage.Top = i
    
    i = i + TargetObject.Height
    
    txtNativeLanguage.Top = i
    txtDistributions.Top = i
    txtSubscriptions.Top = i
    
    txtNativeLanguage.Width = Int(Me.Width / 4)
    txtDistributions.Width = txtNativeLanguage.Width
    txtSubscriptions.Width = txtNativeLanguage.Width
    
    txtDistributions.Left = Label1(0).Left
    txtSubscriptions.Left = Label1(1).Left
    txtNativeLanguage.Left = Label1(2).Left
    cmdXMIT.Left = txtNativeLanguage.Left + txtNativeLanguage.Width + 200
    

End Sub

Private Sub OutputWindow_GotFocus()

        OutputWindow.SelStart = Len(OutputWindow.Text)
        
End Sub

'Private Sub TestOBj_AnswerReady(Index As Integer)
    
 '   WriteToUser "", TestObj(Index).AnswerList
    'Void Msg(CStr(Index), "user", TestObj(Index).AnswerList)
  '  TestObj(Index).AnswerList = ""
    
'End Sub



Private Sub txtMessage_GotFocus()
        
        Me.txtMessage.SelStart = Len(txtMessage.Text)

End Sub

Private Sub txtMessage_KeyPress(KeyAscii As Integer)
        If KeyAscii = vbKeyUp Then KeyAscii = vbKeyPageDown
         If KeyAscii = vbKeyUp Then txtMessage_KeyPress (vbKeyPageDown)
         If KeyAscii <> 13 Then Exit Sub
         txtMessage_KeyPress (vbKeyPageDown)
            
        If Len(txtMessage.Text) > 2 Then txtMessage.AddItem txtMessage.Text
        cmdXMIT_Click
    
End Sub
Private Sub cmdXMIT_Click()
        
        Void Msg(TargetObject.Text, txtFriendlyName.Text, txtMessage.Text)
        txtMessage.Text = ""

End Sub

Private Sub targetobject_KeyPress(KeyAscii As Integer)
        
        If KeyAscii <> 13 Then Exit Sub
        
        'TargetObject.AddItem TargetObject.Text
        'TargetObject_Click

End Sub

Private Sub TargetObject_Click()
    
        'TestObj(Val(TargetObject.Text)).NativeLanguage = Trim(Mid(TargetObject.Text, InStr(TargetObject.Text, " ") + 1))

End Sub



