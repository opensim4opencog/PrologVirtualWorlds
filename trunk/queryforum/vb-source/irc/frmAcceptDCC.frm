VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form AcceptDCC 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Accept DCC"
   ClientHeight    =   2640
   ClientLeft      =   6480
   ClientTop       =   1890
   ClientWidth     =   4935
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
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2640
   ScaleWidth      =   4935
   ShowInTaskbar   =   0   'False
   Begin MSComDlg.CommonDialog cDialog 
      Left            =   2310
      Top             =   1725
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      CancelError     =   -1  'True
   End
   Begin VB.CommandButton Command2 
      Caption         =   "..."
      Height          =   300
      Left            =   4485
      TabIndex        =   11
      Top             =   2250
      Width           =   345
   End
   Begin VB.TextBox txtFullPath 
      Height          =   315
      Left            =   105
      TabIndex        =   10
      Text            =   "C:\projectIRC\<filename>"
      Top             =   2250
      Width           =   4305
   End
   Begin VB.CommandButton Command1 
      Caption         =   "&Decline DCC"
      Height          =   375
      Left            =   3705
      TabIndex        =   5
      Top             =   510
      Width           =   1140
   End
   Begin VB.CommandButton cmdAccept 
      Caption         =   "&Accept DCC"
      Default         =   -1  'True
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   3705
      TabIndex        =   4
      Top             =   90
      Width           =   1140
   End
   Begin VB.Label lblSize 
      Alignment       =   1  'Right Justify
      Caption         =   "<file size>"
      Height          =   225
      Left            =   600
      TabIndex        =   9
      Top             =   1935
      Width           =   4215
   End
   Begin VB.Label Label5 
      AutoSize        =   -1  'True
      Caption         =   "Size &:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   210
      Left            =   120
      TabIndex        =   8
      Top             =   1920
      Width           =   435
   End
   Begin VB.Label lblFile 
      Alignment       =   1  'Right Justify
      Caption         =   "<file name>"
      Height          =   225
      Left            =   600
      TabIndex        =   7
      Top             =   1710
      Width           =   4215
   End
   Begin VB.Label Label4 
      AutoSize        =   -1  'True
      Caption         =   "File &:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   210
      Left            =   120
      TabIndex        =   6
      Top             =   1695
      Width           =   375
   End
   Begin VB.Label Label3 
      AutoSize        =   -1  'True
      Caption         =   "Would you like to Accept this DCC Transfer?"
      Height          =   195
      Left            =   105
      TabIndex        =   3
      Top             =   1380
      Width           =   3165
   End
   Begin VB.Label Label2 
      Caption         =   $"frmAcceptDCC.frx":0000
      Height          =   1020
      Left            =   90
      TabIndex        =   2
      Top             =   270
      Width           =   3465
   End
   Begin VB.Label lblNick 
      Caption         =   "Unknown Nickname"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   240
      Left            =   810
      TabIndex        =   1
      Top             =   75
      Width           =   2640
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "The user, "
      Height          =   195
      Left            =   75
      TabIndex        =   0
      Top             =   75
      Width           =   735
   End
End
Attribute VB_Name = "AcceptDCC"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdAccept_Click()
    
    If FileExists(txtFullPath) Then
        Dim intChoice As Integer, strSuggest
        If Val(lblSize) >= CLng(FileLen(txtFullPath)) Then
            strSuggest = "It appears that you already have the file, or the files conflict.  It is suggested that you click CANCEL and rename the file."
        Else
            strSuggest = "It appears that the file to be sent is smaller than the file on your Computer, and is suggest that you resume the file by click YES, if the files do not conflict."
        End If
        
        intChoice = MsgBox("You have chosen to accept the DCC Transfer, but the directory you chose to save the file contains a file of the same name already." & vbCrLf & "The size of the current file is " & Format(CStr(FileLen(txtFullPath)), "###,###,###,###") & " bytes, while the file you choose to save is " & _
                    Format(lblSize, "###,###,###,###") & " bytes." & vbCrLf & strSuggest & vbCrLf & "Click YES to Resume this file, NO to Overwrite it, and CANCEL to rename the file.", vbCritical Or vbYesNoCancel)
        If intChoice = vbYes Then
            lngDCCStart = FileLen(txtFullPath)
            'Debug.Print
        ElseIf intChoice = vbNo Then
            lngDCCStart = -1
        Else
            Exit Sub
        End If
    Else
        lngDCCStart = -1
    End If
    bAcceptDCC = True
    strDCCFile = txtFullPath
    Unload Me
End Sub


Private Sub Command1_Click()
    bAcceptDCC = False
    Unload Me
End Sub


Private Sub Command2_Click()
    On Error Resume Next
    cDialog.FileName = txtFullPath
    cDialog.ShowSave
    If Err Then Exit Sub
    txtFullPath = cDialog.FileName
End Sub

Private Sub Form_Load()
    lblNick = strDCCNick
    lblFile = strDCCFile
    lblSize = Format(CStr(lngDCCSize), "###,###,###,### bytes")
    txtFullPath = path & lblFile
End Sub


