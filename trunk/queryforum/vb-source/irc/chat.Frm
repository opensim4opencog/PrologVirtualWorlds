VERSION 4.00
Begin VB.Form Form1 
   Caption         =   "Chat module"
   ClientHeight    =   5610
   ClientLeft      =   1140
   ClientTop       =   1710
   ClientWidth     =   6690
   Height          =   6075
   Left            =   1080
   LinkTopic       =   "Form1"
   ScaleHeight     =   5610
   ScaleWidth      =   6690
   Top             =   1305
   Width           =   6810
   Begin VB.Timer Timer2 
      Interval        =   100
      Left            =   5640
      Top             =   3480
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Connect"
      Height          =   375
      Left            =   120
      TabIndex        =   12
      Top             =   3720
      Width           =   1455
   End
   Begin VB.CommandButton Command16 
      Caption         =   "Enter a new room"
      Enabled         =   0   'False
      Height          =   375
      Left            =   3480
      TabIndex        =   11
      Top             =   3720
      Width           =   1575
   End
   Begin VB.TextBox Text2 
      Enabled         =   0   'False
      Height          =   285
      Left            =   120
      MultiLine       =   -1  'True
      TabIndex        =   8
      Top             =   5280
      Width           =   5295
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Send"
      Enabled         =   0   'False
      Height          =   255
      Left            =   5520
      TabIndex        =   7
      Top             =   5280
      Width           =   1095
   End
   Begin VB.TextBox Text3 
      Enabled         =   0   'False
      Height          =   285
      Left            =   120
      TabIndex        =   6
      Top             =   4680
      Width           =   5295
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Send"
      Enabled         =   0   'False
      Height          =   255
      Left            =   5520
      TabIndex        =   5
      Top             =   4680
      Width           =   1095
   End
   Begin VB.TextBox Text1 
      Height          =   495
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   3
      Top             =   0
      Width           =   4935
   End
   Begin VB.ListBox List1 
      Enabled         =   0   'False
      Height          =   1815
      Left            =   5160
      TabIndex        =   2
      Top             =   840
      Width           =   1455
   End
   Begin VB.ListBox List2 
      Height          =   2985
      Left            =   120
      TabIndex        =   1
      Top             =   600
      Width           =   4935
   End
   Begin VB.CheckBox Check1 
      Caption         =   "Private message"
      Enabled         =   0   'False
      Height          =   255
      Left            =   5160
      TabIndex        =   0
      Top             =   0
      Width           =   1575
   End
   Begin VB.Label Label7 
      AutoSize        =   -1  'True
      Caption         =   "Ready for connection"
      Height          =   195
      Left            =   120
      TabIndex        =   13
      Top             =   4200
      Width           =   1530
   End
   Begin VB.Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Send IRC Commands in this box:"
      Height          =   195
      Left            =   120
      TabIndex        =   10
      Top             =   5040
      Width           =   2310
   End
   Begin VB.Label Label2 
      BackStyle       =   0  'Transparent
      Caption         =   "Send messages in this box:"
      Height          =   195
      Left            =   120
      TabIndex        =   9
      Top             =   4440
      Width           =   1920
   End
   Begin VB.Label Label5 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "There are 0 users chatting"
      Height          =   390
      Left            =   5160
      TabIndex        =   4
      Top             =   360
      Width           =   1455
      WordWrap        =   -1  'True
   End
   Begin SocketWrenchCtrl.Socket Socket1 
      Left            =   2760
      Top             =   2640
      _Version        =   65536
      _ExtentX        =   741
      _ExtentY        =   741
      _StockProps     =   0
      AutoResolve     =   -1  'True
      Backlog         =   5
      Binary          =   -1  'True
      Blocking        =   -1  'True
      Broadcast       =   0   'False
      BufferSize      =   0
      HostAddress     =   ""
      HostFile        =   ""
      HostName        =   ""
      InLine          =   0   'False
      Interval        =   0
      KeepAlive       =   0   'False
      Library         =   ""
      Linger          =   0
      LocalPort       =   0
      LocalService    =   ""
      Protocol        =   0
      RemotePort      =   0
      RemoteService   =   ""
      ReuseAddress    =   0   'False
      Route           =   -1  'True
      Timeout         =   0
      Type            =   1
      Urgent          =   0   'False
   End
End
Attribute VB_Name = "Form1"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Dim Uhoh%
Dim OldListCount
Dim Room$
Dim MyName$
Dim TheirName$
Dim Code$
Dim UserName
Dim NL$
Private Sub Check1_Click()
Dim AME
If Check1.Value = 1 Then List1.Enabled = True: Else: List1.Enabled = False
End Sub


Private Sub Command1_Click()
    Dim AB, BC
    If Command1.Caption = "Connect" Then
    Room$ = InputBox$("Enter the room to start in:", "Join a room or create a new one")
    Room$ = "#" + Room$
'   Dim UserName As String
    Dim R&
'   Socket1.HostName = "irc.id-net.fr"
    Socket1.HostAddress = "212.208.103.1"
    Socket1.RemotePort = 6667
    Socket1.LocalPort = 1025
376 Socket1.Connect
    Dim J
    If Uhoh% <> 0 Then J = MsgBox("There was an error connecting. Would you like to try a different local port?", 36, "Error connecting"): Select Case J: Case Is = 6: Socket1.LocalPort = 1035: Uhoh% = 0: GoTo 376: End Select
    R& = Timer + 10
    Do Until Socket1.Connected
    DoEvents
    If Timer > R& Then MsgBox "Connection timeout", 16, "Timed out": Exit Sub
    If Uhoh% <> 0 Then Uhoh% = 0: Exit Sub
    Loop
    Text2.Enabled = True
    Command2.Enabled = True
    Text3.Enabled = True
    Command3.Enabled = True
    Check1.Enabled = True
    DoEvents
'   Socket1.SendLen = 13
'    Socket1.SendLen = 5
'    Socket1.SendData = "/MOTD"
'   Socket1.SendData = "/NICK Compnut"
    DoEvents
'    A = Format(Time$, "h:mm:ss")
3   If InStr(1, LCase$(Command$), "/autorun") <> 0 And UserName <> "" Then
    Else
    UserName = InputBox$("Enter your nickname:", "Name yourself")
    End If
    If UserName = "" Then On Error Resume Next: GoTo 8
    If InStr(1, UserName, " ") Then GoTo 3: MsgBox "No Spaces Allowed in User Name"
    Socket1.SendLen = Len("USER ipt.mstiechat.com a a a a" & Chr$(13) + Chr$(10))
    Socket1.SendData = "USER ipt.mstiechat.com a a a a" & Chr(13) + Chr(10)
    Socket1.SendLen = Len("NICK " & UserName & Chr(13) + Chr(10))
    Socket1.SendData "NICK " & UserName & Chr(13) + Chr(10)
    AB = Text2.Visible
    BC = Text2.Enabled
    Text2.Visible = True: Text2.Enabled = True
    Text2.Text = "JOIN " & Room$ + Chr$(13) + Chr$(10)
    Text2.SetFocus
    DoEvents
    SendKeys "~", True
    Text2.Text = ""
    Text2.Visible = AB
    Text2.Enabled = BC
    Form1.Tag = Form1.Caption
    Form1.Caption = "Connecting..."
    Form1.Enabled = False
    Do Until List1.ListCount > 0
    DoEvents
    Loop
    Form1.Enabled = True
    Form1.Caption = Form1.Tag
    Form1.Tag = ""
    On Error Resume Next
    List2.AddItem "Connected to server at " & Now
    Command16.Enabled = True
    Command1.Caption = "Disconnect"
    Else
8   Text2.Enabled = False
    Command2.Enabled = False
    Text3.Enabled = False
    Command3.Enabled = False
    List1.Enabled = False
    Check1.Value = 0
    Check1.Enabled = False
    Socket1.Action = SOCKET_DISCONNECT
    Label7.Caption = "Socket was disconnected"
    List2.AddItem "Disconnected at " & Now
    Text1.Text = ""
    List1.Clear
    List2.Clear
    Text2.Text = ""
    Text3.Text = ""
    Text2.Enabled = False
    Text3.Enabled = False
    Command2.Enabled = False
    Command3.Enabled = False
    Command16.Enabled = False
    Check1.Value = 0
    Check1.Enabled = False
    List1.Enabled = False
'    If Form1.Tag <> "" Then Form1.Caption = Form1.Tag
    Command1.Caption = "Connect"
    End If
End Sub

Private Sub Command16_Click()
Dim TempRoom$
1 TempRoom$ = InputBox$("Enter the name of the room to enter:", "Change room")
If InStr(1, TempRoom$, " ") Then MsgBox "No Spaces allowed!", 16, "No Spaces": GoTo 1
If TempRoom$ = "" Then Room$ = "#MSquares": Else: Room$ = "#" & TempRoom$: List1.Clear: Socket1.SendLen = Len("JOIN " & Room$ & NL$): Socket1.SendData = "JOIN " & Room$ & NL$
End Sub


Private Sub Form_Load()
    NL$ = Chr$(13) + Chr$(10)
    Label7.Caption = "Initializing Socket..."
    DoEvents
    Label7.Caption = "Setting Address Family..."
    DoEvents
    Socket1.AddressFamily = AF_INET
    Label7.Caption = "Setting Protocol..."
    DoEvents
    Socket1.Protocol = IPPROTO_IP
    Label7.Caption = "Setting Socket type to streamed..."
    DoEvents
    Socket1.SocketType = SOCK_STREAM
    Label7.Caption = "Setting ASCII transfer..."
    DoEvents
    Socket1.Binary = False
    Label7.Caption = "Setting Buffer to 1024..."
    DoEvents
    Socket1.BufferSize = 1024
    Label7.Caption = "Setting Blocking to false"
    DoEvents
    Socket1.Blocking = False
    Label7.Caption = "Ready for connection"
    NL$ = Chr$(13) + Chr$(10)
End Sub


Private Sub Socket1_Read(DataLength As Integer, IsUrgent As Integer)
Label7.Caption = "Recieving Data... " & DataLength & " bytes"
Dim R%, O%, L, OP%, M%, H%, J%, K%
Dim LLBEAN
Socket1.RecvLen = DataLength
Text1.Tag = Socket1.RecvData
Text1.Text = Text1.Text + NL$ + Text1.Tag
If InStr(1, Text1.Tag, "ERROR") > 0 Then Call Command1_Click: MsgBox "You were disconnected because of: " & Text1.Tag, 16, "Disconnected": Exit Sub
If InStr(1, Text1.Tag, "Nickname is already in use.") <> 0 Then Command1.Caption = "Disconnect": Call Command1_Click: Form1.Enabled = True: MsgBox "The nickname you have chosen is currently in use. Please re-connect and choose a different nickname.", 16, "Nickname in use": Exit Sub
R% = InStr(1, Text1.Tag, "= " & Room$ & " :")
If R% <> 0 Then
If InStr(1, Text1.Tag, "@") <> 0 Then Text1.Tag = Left$(Text1.Tag, InStr(1, Text1.Tag, "@") - 1) & Mid$(Text1.Tag, InStr(1, Text1.Tag, "@") + 1, Len(Text1.Tag) - InStr(1, Text1.Tag, "@"))
O% = R% + 13 - (9 - Len(Room$))
'If Mid$(Text1.Tag, O%, 1) = "@" Then O% = O% + 1
OP% = O%
For L = OP% To DataLength
If Mid$(Text1.Tag, L, 1) = " " Then List1.AddItem Trim$(Mid$(Text1.Tag, O%, L - O%)): O% = L
Next
List2.AddItem "<MetaSquarer Host:> " & UserName & " has joined " & Room$
End If
M% = InStr(1, Text1.Tag, "PRIVMSG")
If M% <> 0 Then
H% = InStr(1, Text1.Tag, "!")
J% = 1
Do Until J% = 0
J% = InStr(J% + 1, Text1.Tag, ":")
If J% <> 0 Then K% = J%
Loop
List2.AddItem "<" & Mid$(Text1.Tag, 2, H% - 2) & ">" & " " & Mid$(Text1.Tag, K% + 1, Len(Text1.Tag) - K% - 2)
End If
M% = InStr(1, Text1.Tag, "PART")
If M% <> 0 Then
H% = InStr(1, Text1.Tag, "!")
J% = 1
Do Until J% = 0
J% = InStr(J% + 1, Text1.Tag, ":")
If J% <> 0 Then K% = J%
Loop
On Error Resume Next
For LLBEAN = -1 To List1.ListCount
If Trim$(Mid$(Text1.Tag, 2, H% - 2)) = List1.List(LLBEAN) Then List2.AddItem "<MetaSquarer Host:> " & List1.List(LLBEAN) & " has left the room.": List1.RemoveItem LLBEAN: Exit For
Next
On Error GoTo 0
End If
M% = InStr(1, Text1.Tag, "QUIT")
If M% <> 0 Then
H% = InStr(1, Text1.Tag, "!")
J% = 1
Do Until J% = 0
J% = InStr(J% + 1, Text1.Tag, ":")
If J% <> 0 Then K% = J%
Loop
On Error Resume Next
For LLBEAN = -1 To List1.ListCount
If Trim$(Mid$(Text1.Tag, 2, H% - 2)) = List1.List(LLBEAN) Then List2.AddItem "<MetaSquarer Host:> " & List1.List(LLBEAN) & " has left the room.": List1.RemoveItem LLBEAN: Exit For
Next
On Error GoTo 0
End If
'Add this to the list
M% = InStr(1, Text1.Tag, "JOIN")
If M% <> 0 And InStr(1, Text1.Tag, UserName) = 0 Then
H% = InStr(1, Text1.Tag, "!")
J% = 1
Do Until J% = 0
J% = InStr(J% + 1, Text1.Tag, ":")
If J% <> 0 Then K% = J%
Loop
List2.AddItem "<MetaSquarer Host:> " & Trim$(Mid$(Text1.Tag, 2, H% - 2)) & " has entered the room.": List1.AddItem Trim$(Mid$(Text1.Tag, 2, H% - 2))
End If
Label5.Caption = "There are " & List1.ListCount & " users chatting"
Label7.Caption = "Connected"
End Sub


Private Sub Text2_KeyPress(KeyAscii As Integer)
If Socket1.Connected <> True Then Text2.Enabled = False: Call Command1_Click: Do Until Socket1.Connected = True: DoEvents: Loop: Text2.Enabled = True
If KeyAscii = 13 Then
Socket1.SendLen = Len(Text2.Text & Chr$(13) + Chr$(10))
Socket1.SendData = Text2.Text & Chr$(13) + Chr$(10)
DoEvents
Text2.Text = ""
End If
End Sub


Private Sub Text3_KeyPress(KeyAscii As Integer)
If KeyAscii = 13 Then
If LCase$(Text3.Text) = "//clear" Then List2.Clear: Text3.Text = "": Exit Sub
List2.AddItem "<" & UserName & "> " & Text3.Text
If List1.ListIndex = -1 And Check1.Value = 1 Then MsgBox "Please select someone to send the message to from the list of people on the right.", 16, "No one to send to": Exit Sub
Dim Ryoume%
If Check1.Value = 1 Then Text3.Text = "PRIVMSG " & Trim$(List1.List(List1.ListIndex)) & " :" & Text3.Text: Else: Text3.Text = "PRIVMSG " & Room$ & " :" & Text3.Text
DoEvents
Socket1.SendLen = Len(Text3.Text & NL$)
Socket1.SendData = Text3.Text & NL$
DoEvents
Text3.Text = ""
End If
End Sub


Private Sub Timer2_Timer()
Dim LK
Timer2.Tag = List1.ListCount
If List2.ListCount <> OldListCount Then
On Error Resume Next
For LK = -1 To List2.ListCount - 1
If Len(List2.List(LK)) > 60 Then List2.AddItem Left$(List2.List(LK), 60): List2.AddItem Right$(List2.List(LK), Len(List2.List(LK)) - 60), LK + 2: List2.RemoveItem LK: List2.RemoveItem LK + 2: Exit Sub
Next
OldListCount = List2.ListCount
End If
End Sub


