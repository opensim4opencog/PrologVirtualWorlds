VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form ChannelsList 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Channel Lists"
   ClientHeight    =   4065
   ClientLeft      =   6990
   ClientTop       =   1455
   ClientWidth     =   9375
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4065
   ScaleWidth      =   9375
   ShowInTaskbar   =   0   'False
   Begin MSComctlLib.ListView lvChannels 
      Height          =   4095
      Left            =   -15
      TabIndex        =   0
      Top             =   -15
      Width           =   9405
      _ExtentX        =   16589
      _ExtentY        =   7223
      View            =   3
      LabelEdit       =   1
      Sorted          =   -1  'True
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      AllowReorder    =   -1  'True
      FullRowSelect   =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   0
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Lucida Sans Unicode"
         Size            =   9
         Charset         =   255
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      NumItems        =   3
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Text            =   "Channel Name"
         Object.Width           =   3175
      EndProperty
      BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   1
         Text            =   "User Count"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   2
         Text            =   "Topic"
         Object.Width           =   10583
      EndProperty
   End
End
Attribute VB_Name = "ChannelsList"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public bNeedClear As Boolean
Sub AddChannel(strChannel As String, strUsers As String, strTopic As String)
    Dim lvCount As Integer, var As ListItem
    lvCount = lvChannels.ListItems.Count + 1
    
    If bNeedClear Then
        ChannelsList.bNeedClear = False
        lvChannels.ListItems.Clear
    End If
    
    Set var = lvChannels.ListItems.Add(, "", strChannel, 0, 0)
    
    var.SubItems(1) = strUsers
    var.SubItems(2) = strTopic
    
End Sub


Private Sub lvChannels_DblClick()
    Dim inttemp As Integer
    'inttemp = GetChanIndex(lvchannels.in)
    'If inttemp = -1 Then
    '    Client.SendData "JOIN " & Item.Text
    'End If
End Sub

