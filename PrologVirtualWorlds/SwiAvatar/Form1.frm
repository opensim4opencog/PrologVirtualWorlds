VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   975
   ClientLeft      =   4635
   ClientTop       =   3345
   ClientWidth     =   2265
   LinkTopic       =   "Form1"
   ScaleHeight     =   975
   ScaleWidth      =   2265
   Begin VB.CheckBox chkTest 
      Caption         =   "Allow ShowMessage"
      Height          =   210
      Left            =   90
      TabIndex        =   1
      Top             =   720
      Value           =   1  'Checked
      Width           =   2085
   End
   Begin VB.CommandButton cmdTest 
      Caption         =   "&Test"
      Height          =   510
      Left            =   90
      TabIndex        =   0
      Top             =   90
      Width           =   2085
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim TestObj As Object

Private Sub chkTest_Click()

   TestObj.AllowShowMessage = chkTest.Value = vbChecked
   
End Sub

Private Sub cmdTest_Click()

   TestObj.ShowMessage TestObj.TestProperty
   
End Sub

Private Sub Form_Load()

   Set TestObj = New TestClass
   
   TestObj.AllowShowMessage = True
   TestObj.TestProperty = "Hello World!"
   
End Sub
