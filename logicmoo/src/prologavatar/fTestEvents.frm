VERSION 5.00
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Test"
   ClientHeight    =   1755
   ClientLeft      =   3180
   ClientTop       =   3495
   ClientWidth     =   2985
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1755
   ScaleWidth      =   2985
   Begin VB.CommandButton Command3 
      Caption         =   "Event3 (on the new source interface)"
      Height          =   540
      Left            =   15
      TabIndex        =   2
      Top             =   1200
      Width           =   2940
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Event2 (on the new source interface)"
      Height          =   540
      Left            =   15
      TabIndex        =   1
      Top             =   630
      Width           =   2940
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Event1 (on default source interface)"
      Height          =   540
      Left            =   15
      TabIndex        =   0
      Top             =   60
      Width           =   2940
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim WithEvents DefaultSource As CTestClass
Attribute DefaultSource.VB_VarHelpID = -1
Dim WithEvents EventsSource As CNewEvents
Attribute EventsSource.VB_VarHelpID = -1
Private Sub Command1_Click()

   DefaultSource.RaiseTheEvents 1
   
End Sub

Private Sub Command2_Click()
   
   DefaultSource.RaiseTheEvents 2
   
End Sub


Private Sub Command3_Click()

   DefaultSource.RaiseTheEvents 3
   
End Sub

Private Sub DefaultSource_Event1()

   MsgBox "Event 1 (Default interface)"
   
End Sub

Private Sub EventsSource_Event2()

   MsgBox "Event 2"
   
End Sub


Private Sub EventsSource_Event3(ByVal Param1 As String)

   MsgBox "Event 3: " & Param1

End Sub


Private Sub Form_Load()

   ' Create the test class
   Set DefaultSource = New CTestClass
   
   ' Set the EventsSource to
   ' the test class.
   Set EventsSource = DefaultSource
   
End Sub


