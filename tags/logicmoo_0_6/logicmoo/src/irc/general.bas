Attribute VB_Name = "general"
Option Explicit
Public Channel(1 To 10) As New frmChannel
Public ChannelName(1 To 10) As String
Public ChannelTopic(1 To 10) As String

Public Query(1 To 100) As New frmQuery
Public QueryName(1 To 100) As String

Public RealName  As String
Public email As String
Public NickName As String

Public word(1 To 10) As String


Public connected As Boolean


Public Sub CheckLine(strLine As String)
    Dim i As Integer
    Dim oneline As String
    Static RestLine As String
    
    strLine = RestLine & strLine
    For i = 1 To Len(strLine)
        If Mid(strLine, i, 1) = Chr(13) Or Mid(strLine, i, 1) = Chr(10) Then
            oneline = Mid(strLine, 1, i - 1)
            Call CheckWord(oneline)
            strLine = Mid(strLine, i + 1)

            i = 1
        End If
    Next i
    RestLine = strLine
End Sub
Public Sub CheckWord(strWord As String)
    Dim i As Integer
    Dim x As Integer
    
    'check for carriage return in front of line and delete
    If Mid(strWord, 1, 1) = Chr(13) Or Mid(strWord, 1, 1) = Chr(10) Then
        strWord = Mid(strWord, 2)
    End If
    frmStatus.txtStatus.SelText = "ONELINE: " & strWord & vbCrLf

    
    
    x = 0
    For i = 1 To 10
        word(i) = ""
    Next i
    For i = 1 To Len(strWord)
        If Mid(strWord, i, 1) = Chr(32) Then
            x = x + 1

            If x > 10 Then
                Exit For
            End If
            word(x) = Trim(Mid(strWord, 1, i - 1))
            strWord = LTrim(Mid(strWord, i + 1))
            i = 1
            If IsNumeric(word(2)) = True And x = 3 Then
                Call numeric(word(1), word(2), word(3), strWord)
                Exit Sub
            End If
        End If
    Next i
    Dim parms As String
    For i = 4 To 10
        parms = parms & " " & word(i)
    Next i
    parms = RTrim(LTrim(parms & " " & strWord))
    Call command(word(1), word(2), word(3), parms)
    'frmStatus.txtStatus.SelText = "WORD 2: " & word(2) & " " & word(3) & vbCrLf
    'frmStatus.txtStatus.SelText = "PARMS: " & PARMS & vbCrLf

    
End Sub





Public Sub numeric(server As String, num As String, NickName As String, strLine As String)

On Error GoTo NumericError

    frmStatus.txtStatus.SelText = num & " " & strLine & vbCrLf
    
    Dim i As Integer
    Dim x As Integer
    Dim ix As Integer
    
    'used for "322" to seperate info from PARMS
    Dim mItem As Variant
    Dim xChannelName As String
    Dim xUsers As String
    Dim xTopic As String
    Static ChannelCount As Integer 'count channels on server
    
    
    Select Case num
        'MOTD 372 375 376 378
        Case "372"
            frmMOTD.txtMOTD.SelText = strLine & vbCrLf
            DoEvents
        Case "378"
            frmMOTD.txtMOTD.SelText = strLine & vbCrLf
            DoEvents
        Case "376"
            frmMOTD.Show
        'NAMES 353
        Case "353"
            'channame(3) = first name in list
            'channame(1) = channel name
            Dim ChanName(1 To 100) As String
            'seperate names
            For i = 1 To Len(strLine)
                If Mid(strLine, i, 1) = Chr(32) Then
                    x = x + 1
                    ChanName(x) = Trim(Mid(strLine, 1, i - 1))
                    strLine = LTrim(Mid(strLine, i + 1))
                    If x = 3 Then
                        ChanName(3) = Mid(ChanName(3), 2)
                        'clear name list box in channel and refresh
                        For ix = 1 To 10
                            If LCase(ChannelName(ix)) = LCase(ChanName(2)) And x > 2 Then
                                Channel(ix).lstNames.Clear
                            End If
                        Next ix
                    End If
                    'add name to channel listbox
                    For ix = 1 To 10
                        If LCase(ChannelName(ix)) = LCase(ChanName(2)) And x > 2 Then
                            Channel(ix).lstNames.AddItem ChanName(x)
                        End If
                    Next ix
                    i = 0
                End If
            Next i
            'get last name in 353
            x = x + 1
            ChanName(x) = strLine
        '321 Start List
        '322 list channels #channel #users :topic
        '323 end list
        Case "321"
            ChannelCount = 0
        Case "322"
            ChannelCount = ChannelCount + 1
            frmChannels.Caption = "Cabral Channel List [" & ChannelCount & "]"
            'Tree View Control
            'frmChannels.ChannelView.Nodes.Add = strLine
            
            'seperate CHANNEL USERS :TOPIC
            x = 0
            For i = 1 To Len(strLine)
                If Mid(strLine, i, 1) = Chr(32) Then
                    x = x + 1
                    Select Case x
                        Case 1
                            xChannelName = Mid(strLine, 1, i - 1)
                        Case 2
                            xUsers = Mid(strLine, 1, i - 1)
                        Case 3
                            xTopic = Mid(strLine, 1)
                            Exit For
                    End Select
                    strLine = Mid(strLine, i + 1)
                    i = 0
                End If
            Next i
            'put in listview control (lvxChan control)
            If Len(xChannelName) > 1 Then
                Set frmChannels.lvwChan.SmallIcons = mdiMain.imgChannel
                If Int(xUsers) > 10 Then
                    Set mItem = frmChannels.lvwChan.ListItems.Add(, , xChannelName, , 1)
                Else
                    Set mItem = frmChannels.lvwChan.ListItems.Add(, , xChannelName, , 2)
                End If
                mItem.SubItems(1) = xUsers
                mItem.SubItems(2) = Mid(xTopic, 2)
            End If
            DoEvents
        Case "323"
            frmChannels.Show
    End Select
NumericError:
    If Err.Number <> 0 Then
        MsgBox "Please copy and send the following message to cabral@n-link.com" & vbCrLf & vbCrLf & "Server: " & server & vbCrLf & "Numeric: " & num & vbCrLf & "Nickname: " & NickName & vbCrLf & "String: " & strLine, vbOKOnly, "Bug Found"
        'Resume Next
    End If
End Sub




Public Sub command(Username As String, command As String, target As String, parms As String)
    Dim i As Integer
    Dim x As Integer
    Dim found As Boolean
    
    'chop off the ":" in front of all parameters
    If Left(parms, 1) = ":" Then
        parms = Mid(parms, 2)
    End If
    frmStatus.txtStatus.SelText = command & " " & parms & vbCrLf & "TARGET: " & target & vbCrLf
    
    'break down usrename
    Dim UserEmail
    For i = 1 To Len(Username)
        If Mid(Username, i, 1) = "!" Then
            UserEmail = Mid(Username, i + 1)
            Username = Mid(Username, 1, i - 1)
            If Left(Username, 1) = ":" Then
                Username = Mid(Username, 2)
            End If
        End If
    Next i
    
    
    'this SELECT CASE is for anything that's not a 'command'
    Select Case UCase(Username)
        Case "PING"
            mdiMain.tcp.SendData "PONG " & parms & vbCrLf
            frmStatus.txtStatus.SelColor = RGB(0, 140, 0)
            frmStatus.txtStatus.SelText = "*** PONG " & parms & vbCrLf
            'status window shows server name
            frmStatus.Caption = "Cabral Status: " & parms
            'mdiMain shows [server name]
            mdiMain.Caption = "Cabral IRC [" & parms & "]"
            frmStatus.txtStatus.SelColor = vbBlack
    End Select
    
    Select Case UCase(command)
        Case "JOIN"
            For i = 1 To 10
                'if you join channel then load channel window
                If ChannelName(i) = "" Then
                    If LCase(Username) = LCase(NickName) Then
                        Load Channel(i)
                        ChannelName(i) = LCase(parms)
                        Channel(i).Caption = parms
                        Exit For
                    End If
                End If
                'other users join channel
                'update nick list box
                If LCase(ChannelName(i)) = LCase(parms) Then
                    Channel(i).lstNames.AddItem Username
                    'show in channel that user joined channel
                    Channel(i).txtText.SelColor = RGB(0, 140, 0)
                    Channel(i).txtText.SelText = "*** " & Username & " has joined " & parms & vbCrLf
                    Channel(i).txtText.SelColor = vbBlack
                End If
            Next i
        Case "PART"
            For i = 1 To 10
                'if you leave then unload channel window
                If ChannelName(i) = LCase(parms) Then
                    If LCase(Username) = LCase(NickName) Then
                        Unload Channel(i)
                        ChannelName(i) = ""
                        Exit For
                    End If
                End If
                'other users leave channel
                'update nick list box
                If LCase(ChannelName(i)) = LCase(parms) And parms <> "" Then
                    'cycle through names and remove if found
                    For x = 0 To Channel(i).lstNames.ListCount - 1
                        If Channel(i).lstNames.ListIndex Then
                            If LCase(Channel(i).lstNames.List(x)) = LCase(Username) Or LCase(Channel(i).lstNames.List(x)) = LCase("@" & Username) Then
                                Channel(i).lstNames.RemoveItem (x)
                                'show in channel that user left channel
                                Channel(i).txtText.SelColor = RGB(0, 140, 0)
                                Channel(i).txtText.SelText = "*** " & Username & " has left " & parms & vbCrLf
                                Channel(i).txtText.SelColor = vbBlack
                                'Exit For
                            End If
                        End If
                    Next x
                    
                End If
            Next i
        Case "PRIVMSG"
            If Left(target, 1) = "#" Then
                For i = 1 To 10
                    If ChannelName(i) = LCase(target) Then
                        'if it's a channel action
                        If LCase(Left(parms, 7)) = LCase("ACTION") Then
                            Channel(i).txtText.SelColor = RGB(140, 0, 140)
                            Channel(i).txtText.SelText = "* "
                            Channel(i).txtText.SelBold = True
                            Channel(i).txtText.SelText = Username
                            Channel(i).txtText.SelBold = False
                            Channel(i).txtText.SelText = " " & Mid(parms, 8) & vbCrLf
                            Channel(i).txtText.SelColor = vbBlack
                        Else
                            Channel(i).txtText.SelText = "<"
                            Channel(i).txtText.SelBold = True
                            Channel(i).txtText.SelText = Username
                            Channel(i).txtText.SelBold = False
                            Channel(i).txtText.SelText = "> " & parms & vbCrLf
                        End If
                    Else
                        'if exited channel before PART msg then show channel text in status
                        'bottom line echos 10 times in status (fix)
                        'frmStatus.txtStatus.SelText = target & ": <" & Username & "> " & parms & vbCrLf
                    End If
                Next i
            Else
                found = False
                
                For i = 1 To 100
                    If LCase(Username) = LCase(QueryName(i)) Then
                        found = True
                        Exit For
                    End If
                Next i
                If found = False Then
                    For i = 1 To 100
                        If QueryName(i) = "" Then
                            'load query window
                            Load Query(i)
                            Query(i).Caption = Username
                            QueryName(i) = Username
                            Exit For
                        End If
                    Next i
                End If
                
                For i = 1 To 100
                    If LCase(QueryName(i)) = LCase(Username) Then
                        Query(i).txtQuery.SelText = "<"
                        Query(i).txtQuery.SelBold = True
                        Query(i).txtQuery.SelColor = vbRed
                        Query(i).txtQuery.SelText = Username
                        Query(i).txtQuery.SelBold = False
                        Query(i).txtQuery.SelColor = vbBlack
                        Query(i).txtQuery.SelText = "> " & parms & vbCrLf
                    End If
                Next i
            End If
        Case "NICK"
            'Update your nickname variable
            If LCase(Username) = LCase(NickName) Then
                NickName = parms
            End If
        Case "NOTICE"
            'channel notice
            If Left(target, 1) = "#" Then
                For i = 1 To 10
                    'cycle through channel names and if found a match then
                    'writing to channel window
                    If LCase(target) = LCase(ChannelName(i)) Then
                        Channel(i).txtText.SelColor = RGB(140, 0, 0)
                        Channel(i).txtText.SelText = target
                        Channel(i).txtText.SelColor = vbBlack
                        Channel(i).txtText.SelText = ": <"
                        Channel(i).txtText.SelBold = True
                        Channel(i).txtText.SelText = Username
                        Channel(i).txtText.SelBold = False
                        Channel(i).txtText.SelText = "> " & parms & vbCrLf
                    Else
                        'show channel NOTICE to status window
                        
                    End If
                Next i
            Else
                'personal NOTICE to you
                'query window for username not found yet
                found = False
                
                'loop to find an already open query window
                For i = 1 To 100
                    If LCase(QueryName(i)) = LCase(Username) Then
                        found = True
                    End If
                Next i
                
                'create query window if not already loaded
                If found = False Then
                    For i = 1 To 100
                        If QueryName(i) = "" Then
                            'load query window
                            Load Query(i)
                            Query(i).Caption = Username
                            QueryName(i) = Username
                            Exit For
                        End If
                    Next i
                End If
                'finally show NOTICE in query window
                For i = 1 To 100
                    If LCase(QueryName(i)) = LCase(Username) Then
                        Query(i).txtQuery.SelColor = RGB(140, 0, 0)
                        Query(i).txtQuery.SelBold = True
                        Query(i).txtQuery.SelText = "NOTICE"
                        Query(i).txtQuery.SelBold = False
                        Query(i).txtQuery.SelColor = vbBlack
                        Query(i).txtQuery.SelText = ": " & parms & vbCrLf
                    End If
                Next i
            End If
        Case "QUIT"
            'cycle through each window since the QUIT message
            'tells you no channel or anything
            For i = 1 To 10
                'if channel exsists (should have name)
                If ChannelName(i) <> "" Then
                    'cycle through each listbox on each channel window
                    For x = 0 To Channel(i).lstNames.ListCount - 1
                        'if QUIT name matches lstnames(name) then remove
                        If LCase(Channel(i).lstNames.List(x)) = LCase(Username) Or LCase(Channel(i).lstNames.List(x)) = LCase("@" & Username) Then
                            'remove from list box for certain window
                            Channel(i).lstNames.RemoveItem (x)
                            'Display quit in channel
                            Channel(i).txtText.SelColor = RGB(0, 0, 140)
                            Channel(i).txtText.SelText = "*** " & Username & " has QUIT (" & parms & ")" & vbCrLf
                            Channel(i).txtText.SelColor = vbBlack
                        End If
                    Next x
                End If
            Next i
    End Select
End Sub

