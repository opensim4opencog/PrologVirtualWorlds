VERSION 5.00
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form DCCSend 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "DCC Transfer"
   ClientHeight    =   2535
   ClientLeft      =   915
   ClientTop       =   8490
   ClientWidth     =   3630
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
   MDIChild        =   -1  'True
   MinButton       =   0   'False
   ScaleHeight     =   2535
   ScaleWidth      =   3630
   ShowInTaskbar   =   0   'False
   Begin MSComctlLib.ProgressBar progress 
      Height          =   135
      Left            =   75
      TabIndex        =   17
      Top             =   2115
      Width           =   3480
      _ExtentX        =   6138
      _ExtentY        =   238
      _Version        =   393216
      Appearance      =   0
      Scrolling       =   1
   End
   Begin MSWinsockLib.Winsock sock 
      Left            =   1620
      Top             =   1380
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
   End
   Begin VB.Label lblStat 
      Alignment       =   1  'Right Justify
      Caption         =   "Connecting..."
      Height          =   210
      Left            =   1290
      TabIndex        =   16
      Top             =   2295
      Width           =   2250
   End
   Begin VB.Label lblTimeLeft 
      Alignment       =   1  'Right Justify
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "N/A"
      ForeColor       =   &H00000000&
      Height          =   195
      Left            =   3285
      TabIndex        =   15
      Top             =   1320
      Width           =   270
   End
   Begin VB.Label Label7 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Time Left &:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   210
      Left            =   45
      TabIndex        =   14
      Top             =   1320
      Width           =   885
   End
   Begin VB.Shape Shape2 
      BackStyle       =   1  'Opaque
      BorderColor     =   &H00808080&
      Height          =   195
      Left            =   45
      Top             =   2085
      Width           =   3540
   End
   Begin VB.Label lblBps 
      Alignment       =   1  'Right Justify
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "N/A"
      ForeColor       =   &H00000000&
      Height          =   195
      Left            =   3285
      TabIndex        =   13
      Top             =   1800
      Width           =   270
   End
   Begin VB.Label Label6 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Bytes/Second &:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   210
      Left            =   45
      TabIndex        =   12
      Top             =   1800
      Width           =   1215
   End
   Begin VB.Label lblSentRcvd 
      Alignment       =   1  'Right Justify
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "N/A"
      ForeColor       =   &H00000000&
      Height          =   195
      Left            =   3285
      TabIndex        =   11
      Top             =   1560
      Width           =   270
   End
   Begin VB.Label lblSR 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Sent/Rcvd &:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   210
      Left            =   45
      TabIndex        =   10
      Top             =   1560
      Width           =   900
   End
   Begin VB.Label lblTimeElapsed 
      Alignment       =   1  'Right Justify
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "N/A"
      ForeColor       =   &H00000000&
      Height          =   195
      Left            =   3285
      TabIndex        =   9
      Top             =   1080
      Width           =   270
   End
   Begin VB.Label Label3 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Time Elapsed &:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   210
      Left            =   45
      TabIndex        =   8
      Top             =   1080
      Width           =   1200
   End
   Begin VB.Label lblSize 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "N/A"
      ForeColor       =   &H00FFFFFF&
      Height          =   240
      Left            =   540
      TabIndex        =   7
      Top             =   735
      Width           =   3015
   End
   Begin VB.Label Label4 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
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
      ForeColor       =   &H00FFFFFF&
      Height          =   210
      Left            =   45
      TabIndex        =   6
      Top             =   735
      Width           =   435
   End
   Begin VB.Label lblFile 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "N/A"
      ForeColor       =   &H00FFFFFF&
      Height          =   420
      Left            =   540
      TabIndex        =   5
      Top             =   285
      Width           =   3015
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
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
      ForeColor       =   &H00FFFFFF&
      Height          =   210
      Left            =   45
      TabIndex        =   4
      Top             =   285
      Width           =   375
   End
   Begin VB.Label lblNick 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "N/A"
      ForeColor       =   &H00FFFFFF&
      Height          =   195
      Left            =   1830
      TabIndex        =   3
      Top             =   45
      Width           =   1725
   End
   Begin VB.Label lblToFrom 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "From &:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   210
      Left            =   1335
      TabIndex        =   2
      Top             =   45
      Width           =   525
   End
   Begin VB.Label lblType 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "N/A"
      ForeColor       =   &H00FFFFFF&
      Height          =   195
      Left            =   540
      TabIndex        =   1
      Top             =   45
      Width           =   705
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Type &:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   210
      Left            =   45
      TabIndex        =   0
      Top             =   45
      Width           =   495
   End
   Begin VB.Shape Shape1 
      BackColor       =   &H8000000C&
      BackStyle       =   1  'Opaque
      BorderStyle     =   0  'Transparent
      Height          =   1035
      Left            =   -15
      Top             =   -15
      Width           =   3675
   End
End
Attribute VB_Name = "DCCSend"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public ID               As String
Public lngLocalPort     As Long
Public lngRemotePort    As Long
Public lngFileSize      As Long
Public lngIp            As Long
Public newBuffer        As Boolean 'not used, but...
Public strFile          As String
Public strNick          As String
Public lngTimeElapsed   As Long
Public lngTimeLeft      As Long
Public lngSentRcvd      As Long
Public lngBps           As Long
Public intDCCType       As Integer

Dim lngStartTime As Long
Dim lngCurTime As Long
Public Sub msgit()
    MsgBox sock.RemoteHost
End Sub


Public Sub StartResume(lngPort As Long)
    'MsgBox "ehh"
    sock.Close
    sock.RemotePort = lngPort
    On Error Resume Next
    sock.Connect

End Sub

Public Sub UpdateInfo()
    Dim strTimeElapsed As String, lngLeft As Long
    strTimeElapsed = Duration(lngTimeElapsed / 1000)
    
    lngCurTime = GetTickCount
    lngTimeElapsed = lngCurTime - lngStartTime
    lblTimeElapsed.Caption = strTimeElapsed
    On Error Resume Next
    lngBps = (lngSentRcvd / (lngTimeElapsed / 1000))
    lblBps = Format(lngBps, "###,###,###,###")
    
    If intDCCType = 1 Then
        lblType = "Send"
        lblToFrom = "To &:"
        lblSR = "Sent &:"
    Else
        lblType = "Receive"
        lblToFrom = "From &:"
        lblSR = "Rcvd &:"
    End If
    
    lblNick = strNick
    lblFile = strFile
    lblSize = Format(lngFileSize, "###,###,###,###")
    lblTimeLeft = lngTimeLeft
    lblSentRcvd = Format(lngSentRcvd, "###,###,###,###")
    
    Dim lngPerWidth As Long, intPercent As Integer, drawWidth As Long
    intPercent = lngSentRcvd / lngFileSize * 100
    Me.Caption = "DCC Transfer - " & intPercent & "%"
    progress.Max = lngFileSize
    progress.Value = lngSentRcvd
    
    lngLeft = (lngFileSize - lngSentRcvd) / lngBps
    lblTimeLeft = Duration(lngLeft)
        
End Sub


Private Sub Form_Activate()
    Dim i As Integer
    i = GetWindowIndex("DCC " & lblType & " (" & strNick & " : " & strFile & ")")
    SetWinFocus i
    Client.intActive = i
    Client.intHover = -1
    DoEvents
    Client.DrawToolbar
End Sub

Private Sub Form_Unload(Cancel As Integer)
    If Val(Me.Tag) = intDCCSends Then intDCCSends = intDCCSends - 1
    On Error Resume Next
    Unload DCCSends(Me.Tag)
    Me.Tag = ""
    Unload Me
    Client.DrawToolbar
End Sub


Private Sub sock_Close()
    lblStat = "Closed"
End Sub

Private Sub sock_Connect()
    lblStat = "Connected"
    
    If FileExists(strDCCFile) And lngSentRcvd = 0 Then
        Kill path & strFile
    End If
    
    lngStartTime = GetTickCount
End Sub


Private Sub sock_ConnectionRequest(ByVal requestID As Long)
    sock.Close
    sock.Accept requestID
    lblStat = "Accepted connection"
End Sub

Private Sub sock_DataArrival(ByVal bytesTotal As Long)
    If lblType = "Receive" Then
        lblStat = "Incoming packet : " & Format(bytesTotal, "###,###,###") & " bytes"
        Dim dat As String
        sock.GetData dat, vbString
        
        Open strDCCFile For Binary Access Write As #1
            Put #1, (LOF(1) + 1), dat
        Close #1
        
        lngSentRcvd = lngSentRcvd + Len(dat)
        UpdateInfo
        sock.SendData BytesToChars(lngSentRcvd)
        If lngSentRcvd >= lngFileSize Then
            TimeOut 1
            sock.Close
            lblStat = "File Complete"
        End If
        On Error Resume Next
    Else
    End If
End Sub

Private Sub sock_Error(ByVal Number As Integer, Description As String, ByVal Scode As Long, ByVal Source As String, ByVal HelpFile As String, ByVal HelpContext As Long, CancelDisplay As Boolean)
    lblStat = "ERROR: " & Description
End Sub


