VERSION 5.00
Begin VB.Form frmLogin1 
   Caption         =   "Login"
   ClientHeight    =   3870
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7425
   LinkTopic       =   "Form1"
   ScaleHeight     =   3870
   ScaleWidth      =   7425
   StartUpPosition =   3  'Windows Default
End
Attribute VB_Name = "frmLogin1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Function GetUserName Lib "advapi32.dll" Alias "GetUserNameA" (ByVal lpbuffer As String, nSize As Long) As Long


Public OK As Boolean
Private Sub Form_Load()
    Dim sBuffer As String
    Dim lSize As Long


    LoadResStrings Me


    sBuffer = Space$(255)
    lSize = Len(sBuffer)
    Call GetUserName(sBuffer, lSize)
    If lSize > 0 Then
        txtUserName.Text = Left$(sBuffer, lSize)
    Else
        txtUserName.Text = vbNullString
    End If
End Sub



Private Sub cmdCancel1_Click()
    OK = False
    Me.Hide
End Sub


Private Sub cmdOK_Click1()
    'ToDo: create test for correct password
    'check for correct password
    If txtPassword.Text = "" Then
        OK = True
        Me.Hide
    Else
        MsgBox "Invalid Password, try again!", , "Login"
        txtPassword.SetFocus
        txtPassword.SelStart = 0
        txtPassword.SelLength = Len(txtPassword.Text)
    End If
End Sub



Private Sub fraMainFrame_DragDrop(Source As Control, X As Single, Y As Single)

End Sub

