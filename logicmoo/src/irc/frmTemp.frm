VERSION 5.00
Begin VB.Form Temp 
   Caption         =   "Temp"
   ClientHeight    =   1755
   ClientLeft      =   2475
   ClientTop       =   5910
   ClientWidth     =   6570
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   ScaleHeight     =   1755
   ScaleWidth      =   6570
   Begin VB.ListBox List1 
      Height          =   1035
      Left            =   135
      TabIndex        =   2
      Top             =   480
      Width           =   4785
   End
   Begin VB.CommandButton cmdParse 
      Caption         =   "&Parse"
      Height          =   570
      Left            =   5115
      TabIndex        =   1
      Top             =   510
      Width           =   1335
   End
   Begin VB.TextBox Text1 
      Height          =   330
      Left            =   135
      TabIndex        =   0
      Text            =   "+vv-o+b vcv sappy vcv god"
      Top             =   90
      Width           =   6330
   End
End
Attribute VB_Name = "Temp"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdParse_Click()
    Dim strModes() As String, strChar As String
    Dim i As Integer, intParam As Integer
    Dim bAdd As Boolean
    
    bAdd = True
    strModes = Split(Text1.Text, " ")
    For i = 1 To Len(strModes(0))
        strChar = Mid(strModes(0), i, 1)
        Select Case strChar
            Case "+"
                bAdd = True
            Case "-"
                bAdd = False
            Case "v", "b", "o", "h"
                intParam = intParam + 1
                'DoMode badd, strChar, strModes(intparam)
                List1.AddItem bAdd & ":" & strChar & " " & strModes(intParam)
            Case Else
                'DoMode badd, strChar, ""
                List1.AddItem bAdd & ":" & strChar
        End Select
    Next i
            
End Sub


