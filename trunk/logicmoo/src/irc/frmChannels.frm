VERSION 5.00
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.2#0"; "COMCTL32.OCX"
Begin VB.Form frmChannels 
   Caption         =   "Cabral Chanel List"
   ClientHeight    =   3210
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5070
   Icon            =   "frmChannels.frx":0000
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   3210
   ScaleWidth      =   5070
   Begin ComctlLib.ListView lvwChan 
      Height          =   2895
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   4815
      _ExtentX        =   8493
      _ExtentY        =   5106
      View            =   3
      Sorted          =   -1  'True
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      _Version        =   327682
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   0
   End
End
Attribute VB_Name = "frmChannels"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False


Private Sub Form_Load()
    ' Clear the ColumnHeaders collection.
    lvwChan.ColumnHeaders.Clear
    ' Add four ColumnHeaders.
    lvwChan.ColumnHeaders.Add , , "Channel", 3000
    lvwChan.ColumnHeaders.Add , , "Users"
    lvwChan.ColumnHeaders.Add , , "Title", 3500
    
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    lvwChan.Move lvwChan.Left, lvwChan.Top, Me.Width - 100, Me.Height - 400
End Sub

Private Sub lvwChan_ItemClick(ByVal Item As ComctlLib.ListItem)
    mdiMain.tcp.SendData "JOIN " & Item.Text & vbCrLf
End Sub
