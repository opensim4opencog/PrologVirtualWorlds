Attribute VB_Name = "modIRC"

Public Sub ActionAll(strText As String)
    For i = 1 To intChannels
        If Channels(i).strName <> "" Then
            PutData Channels(i).DataIn, strColor & "06" & strMyNick & " " & strText
            Client.SendData "PRIVMSG " & Channels(i).strName & " :" & strAction & "ACTION " & strText & strAction
        End If
    Next i
End Sub


Public Function BytesToChars(ByVal lngBytes As Long) As String
     Dim strBuffer As String
     strBuffer$ = Chr$(lngBytes& Mod 256)
     lngBytes& = Int(lngBytes& / 256)
     strBuffer$ = Chr$(lngBytes& Mod 256) & strBuffer$
     lngBytes& = Int(lngBytes& / 256)
     strBuffer$ = Chr$(lngBytes& Mod 256) & strBuffer$
     lngBytes& = Int(lngBytes& / 256)
     strBuffer$ = Chr$(lngBytes&) & strBuffer$
     
     BytesToChars = strBuffer$
End Function

Function Duration(lngSeconds As Long) As String
    If lngSeconds = 0 Then Duration = "0 seconds": Exit Function
    Dim yrs As Long, mnths As Long, dys As Long, hrs As Long, mns As Long, scs As Long
    
    yrs = Int(lngSeconds / 31557600) '# of seconds in a year
    lngSeconds = lngSeconds Mod 31557600
    
    mnths = lngSeconds / 2629800    '# of seconds in month
    lngSeconds = lngSeconds Mod 2629800
    
    dys = lngSeconds / 86400    '# of seconds in a day
    lngSeconds = lngSeconds Mod 86400
    
    hrs = lngSeconds / 3600     '# of seconds in an hour
    lngSeconds = lngSeconds Mod 3600
    
    mns = Int(lngSeconds / 60)
    scs = lngSeconds Mod 60
    
'    MsgBox yrs & " " & mnths & " " & dys & " " & hrs & " " & mns & " " & scs
    Duration = LeftR(SV(yrs, "year") & _
               SV(mnths, "month") & _
               SV(dys, "day") & _
               SV(hrs, "hour") & _
               SV(mns, "minute") & _
               SV(scs, "second"), 1)
    
               
End Function

Sub EchoActive(strText As String, Optional intColor As Integer = 1)
    On Error Resume Next
    PutData Client.ActiveForm.DataIn, strColor & intColor & strText
    If Err Then PutData Status.DataIn, strColor & intColor & strText
End Sub

Function GetDCCSendIndex(strID As String)
    Dim i As Integer

    For i = 1 To intdccsemds
        If LCase(DCCSends(i).ID) = LCase(strID) Then
            GetDCCSendIndex = i
            Exit Function
        End If
    Next i
    GetDCCSendIndex = -1

End Function

Public Function NewDCCSend(typDCC As DCC_INFO)
    
    i = GetDCCSendIndex(typDCC.ID)
    'If i <> -1 Then
    '    DCCSends(i).SetFocus
    '    Exit Sub
    'End If

    For i = 1 To intDCCSends
        If DCCSends(i).ID = "" Then
            NewDCCSend = i
            DCCSends(i).ID = typDCC.ID
            DCCSends(i).lngRemotePort = typDCC.Port
            DCCSends(i).strFile = typDCC.File
            DCCSends(i).strNick = typDCC.Nick
            DCCSends(i).intDCCType = typDCC.type
            DCCSends(i).lngFileSize = typDCC.Size
            DCCSends(i).sock.RemoteHost = typDCC.IP
            DCCSends(i).UpdateInfo
            DCCSends(i).Show
            DCCSends(i).Tag = i
            DCCSends(i).sock.RemotePort = typDCC.Port
            Exit Function
        End If
    Next i
    
    intDCCSends = intDCCSends + 1
    Set DCCSends(intDCCSends) = New DCCSend
    NewDCCSend = intDCCSends
    DCCSends(intDCCSends).ID = typDCC.ID
    DCCSends(intDCCSends).lngRemotePort = typDCC.Port
    DCCSends(intDCCSends).strFile = typDCC.File
    DCCSends(intDCCSends).strNick = typDCC.Nick
    DCCSends(intDCCSends).intDCCType = typDCC.type
    DCCSends(intDCCSends).lngFileSize = typDCC.Size
    DCCSends(intDCCSends).sock.RemoteHost = typDCC.IP
    DCCSends(intDCCSends).sock.RemotePort = typDCC.Port
    DCCSends(intDCCSends).UpdateInfo
    DCCSends(intDCCSends).Show
    DCCSends(intDCCSends).Tag = intDCCSends
End Function


Sub PutText(RTF As RichTextBox, strData As String)
    
    PutData RTF, strData
    Exit Sub
    '* Not Finished
    If strData = "" Then Exit Sub
    'DoEvents
    Dim i As Long, Length As Integer, strChar As String
    Dim strBuffer As String, j As Long, colorTable As String, strelse As String
    Dim strRTF As String, bold As Boolean, underline As Boolean, clr As String
    
    If strFont = "" Then strFont = "IBMPC"
    
    strRTF = "{\rtf1\ansi\ansicpg1252\deff0\deftab720{\fonttbl{\f0\fswiss " & strFont & ";}{\f1\froman\fcharset2 Symbol;}{\f2\fswiss " & strFont & ";}{\f3\fswiss " & strFont & ";}}" & vbCrLf
    colorTable = "{\colortbl\red0\green0\blue0;\red0\green0\blue255;\red255\green0\blue0;\red0\green0\blue127;\red0\green127\blue0;\red255\green255\blue255;\red127\green0\blue127;\red255\green127\blue0;\red255\green255\blue0;\red127\green0\blue0;\red0\green148\blue144;\red0\green255\blue255;\red0\green255\blue0;\red92\green92\blue92;\red184\green184\blue184;\red255\green0\blue255;}" & vbCrLf
    strRTF = strRTF & colorTable & "\deflang1033\pard\plain\f2\fs17 "
    
    strData = " " & strData & vbCrLf
    Length = Len(strData)
    i = 1
    RTF.SelStart = Len(RTF.Text)
    RTF.SelFontName = strFont
    RTF.SelColor = lngForeColor
    RTF.SelBold = False
    RTF.SelUnderline = False
    RTF.SelStrikeThru = False
    'DoEvents
    clr = lngForeColor
    Do
        strChar = Mid(strData, i, 1)
        Select Case strChar
            Case strBold
                bold = Not bold
                i = i + 1
            Case strUnderline
                'RTF.SelStart = Len(RTF.Text)
                'RTF.SelText = strBuffer
                'strBuffer = ""
                'RTF.SelUnderline = Not RTF.SelUnderline
                underline = Not underline
                i = i + 1
            Case strReverse
                'RTF.SelStart = Len(RTF.Text)
                'RTF.SelText = strBuffer
                'strBuffer = ""
                'RTF.SelStrikeThru = Not RTF.SelStrikeThru
                i = i + 1
            Case strColor
                'RTF.SelStart = Len(RTF.Text)
                'RTF.SelText = strBuffer
                'strBuffer = ""
                i = i + 1
                Do Until Not ValidColorCode(strBuffer) Or i > Length
                    strBuffer = strBuffer & Mid(strData, i, 1)
                    i = i + 1
                Loop
                strBuffer = LeftR(strBuffer, 1)
                'RTF.SelStart = Len(RTF.Text)
                If strBuffer = "" Then strBuffer = "0"
                clr = strBuffer
                i = i - 1
                'strBuffer = ""
            Case Else
                strelse = ""
                strBuffer = ""
                If bold Then strelse = strelse & "\b"
                If underline Then strelse = strelse & "\ul"
                strelse = strelse & "\cf" & clr
                'strBuffer = strBuffer & strChar
                strBuffer = "\plain\f11\fs17" & strelse & " " & strChar
                strRTF = strRTF & strBuffer
                i = i + 1
        End Select
    Loop Until i > Length
    strRTF = strRTF & " " & vbCrLf & "\par }"
    'MsgBox strRTF
    RTF.SelStart = Len(RTF.Text)
    RTF.SelRTF = strRTF
    'If strBuffer <> "" Then
    '        RTF.SelStart = Len(RTF.Text)
    '        RTF.SelText = strBuffer
    '        strBuffer = ""
    'End If
    'RTF.SelBold = False
    'RTF.SelUnderline = False
    'RTF.SelStrikeThru = False
    RTF.SelStart = Len(RTF.Text)
    RTF.SelText = vbCrLf
End Sub


Public Function RichWordOver(rch As RichTextBox, x As Single, Y As Single) As String
    Dim pt As POINTAPI
    Dim pos As Long
    Dim start_pos As Integer
    Dim end_pos As Integer
    Dim ch As String
    Dim txt As String
    Dim txtlen As Integer
    Dim i As Integer, j As Integer

    ' Convert the position to pixels.
    pt.x = x \ Screen.TwipsPerPixelX
    pt.Y = Y \ Screen.TwipsPerPixelY

    ' Get the character number
    On Error Resume Next
   ' pos = SendMessage(rch.hwnd, EM_CHARFROMPOS, 0&, pt)
    'Client.Caption = "~" & pos & "~" & Len(rch.Text)
    
    'Exit Function
    If pos <= 0 Or pos >= Len(rch.Text) Then
        RichWordOver = ""
        Exit Function
    End If
    
    txt = ""
    For i = pos To 1 Step -1
        ch = Mid(rch.Text, i, 1)
        'If i Mod 100 = 3 Then DoEvents
        If ch = " " Or _
           ch = "," Or _
           ch = "(" Or _
           ch = ")" Or _
           ch = "]" Or _
           ch = "[" Or _
           ch = "{" Or _
           ch = """" Or _
           ch = "'" Or _
           ch = Chr(9) Or _
           ch = "}" Then
            start_pos = i
            GoTo haha
        End If
    Next i
haha:
    txt = ""
    For i = pos To Len(rch.Text)
        ch = Mid(rch.Text, i, 1)
        If ch = " " Or _
           ch = "," Or _
           ch = "(" Or _
           ch = ")" Or _
           ch = "]" Or _
           ch = "[" Or _
           ch = "{" Or _
           ch = "}" Or _
           ch = """" Or _
           ch = "'" Or _
           ch = Chr(9) Then
            end_pos = i
            Exit For
        End If
    Next i
    
    If end_pos > Len(rch.Text) Or end_pos <= 0 Then end_pos = Len(rch.Text)
    
    RichWordOver = RightR(Replace(Mid(rch.Text, start_pos, end_pos - start_pos), Chr(13), ""), 1)
End Function



Function AnsiColor(intColNum As Integer) As Long
    Select Case intColNum
        Case 0: AnsiColor = RGB(255, 255, 255)
        Case 1: AnsiColor = RGB(0, 0, 0)
        Case 2: AnsiColor = RGB(0, 0, 127)
        Case 3: AnsiColor = RGB(0, 127, 0)
        Case 4: AnsiColor = RGB(255, 0, 0)
        Case 5: AnsiColor = RGB(127, 0, 0)
        Case 6: AnsiColor = RGB(127, 0, 127)
        Case 7: AnsiColor = RGB(255, 127, 0)
        Case 8: AnsiColor = RGB(255, 255, 0)
        Case 9: AnsiColor = RGB(0, 255, 0)
        Case 10: AnsiColor = RGB(0, 148, 144)
        Case 11: AnsiColor = RGB(0, 255, 255)
        Case 12: AnsiColor = RGB(0, 0, 255)
        Case 13: AnsiColor = RGB(255, 0, 255)
        Case 14: AnsiColor = RGB(92, 92, 92)
        Case 15: AnsiColor = RGB(184, 184, 184)
        Case Else: AnsiColor = RGB(0, 0, 0)
    End Select
End Function



Sub ChangeNick(strOldNick As String, strNewNick As String)
    Dim i As Integer, bChangedQuery As Boolean, inttemp As Integer
    
    For i = 1 To intChannels
    
        
        If Channels(i).InChannel(strOldNick) Then
            
            'change in queries :)
            If Not bChangedQuery Then
                inttemp = GetQueryIndex(strOldNick)
                If inttemp <> -1 Then
                    Queries(inttemp).lblNick = strNewNick
                    Queries(inttemp).strNick = strNewNick
                    Queries(inttemp).Caption = strNewNick
                    bChangedQuery = True
                End If
            End If
            
            'change in channel :)
            If Channels(i).strName <> "" Then
                Channels(i).ChangeNck strOldNick, strNewNick
            End If
        End If
    Next i
End Sub

Function Combine(arrItems() As String, intStart As Integer, intEnd As Integer) As String
    '* This returns the given parameter range as one string
    '* -1 specified as strEnd means from intStart to the last parameter
    '* Ex: strParams(1) is "#mIRC", then params 2 thru 16 are the nicks
    '*     of the users in the channel, simply do something like this
    '*     strNames = Params(2, -1)
    '*     that would return all the nicks into one string, similar to mIRC's
    '*      $#- identifier ($2-) in this case
    
    '* Declare variables
    Dim strFinal As String, intLast As Integer, i As Integer
    
    '* check for bad parameters
    If intStart < 1 Or intEnd > UBound(arrItems) + 1 Then Exit Function
    
    '* if intEnd = -1 then set it to param count
    If intEnd = -1 Then intLast = UBound(arrItems) + 1 Else intLast = intEnd
        
    For i = intStart To intLast
        strFinal = strFinal & arrItems(i - 1)
        If i <> intLast Then strFinal = strFinal & " "
    Next i
    
    Combine = Replace(strFinal, vbCr, "")
    strFinal = ""
End Function

Function DisplayNick(nckNick As Nick) As String
    Dim strPre As String
    If nckNick.voice Then strPre = "+"
    If nckNick.helper Then strPre = "%"
    If nckNick.op Then strPre = "@"
    DisplayNick = strPre & nckNick.Nick
End Function

Sub DoMode(strChannel As String, bAdd As Boolean, strMode As String, strParam As String)
    
    If strChannel = strMyNick Then
        If bAdd Then
            Client.AddMode strMode, bAdd
        Else
            Client.RemoveMode strMode
        End If
        Exit Sub
    End If
    
    Dim intX As Integer, i As Integer
    intX = GetChanIndex(strChannel)
    If intX = -1 Then Exit Sub
    
    Select Case strMode
        Case "v"
            Channels(intX).SetVoice strParam, bAdd
            
        Case "o"
            Channels(intX).SetOp strParam, bAdd
            If bAdd Then
                If strParam = strMyNick Then Channels(intX).rtbTopic.Tag = ""
            Else
                If strParam = strMyNick Then Channels(intX).rtbTopic.Tag = "locked"
            End If
        Case "h"
            Channels(intX).SetHelper strParam, bAdd
        Case "b"
        Case "k"
            If bAdd = True Then
                Channels(intX).strKey = strParam
                Channels(intX).AddMode strMode, bAdd
            Else
                Channels(intX).strKey = ""
                Channels(intX).RemoveMode strMode
            End If
        Case "l"
            If bAdd = True Then
                Channels(intX).intLimit = CInt(strParam)
                Channels(intX).AddMode strMode, bAdd
            Else
                Channels(intX).intLimit = 0
                Channels(intX).RemoveMode strMode
            End If
        Case Else
            If bAdd = True Then
                Channels(intX).AddMode strMode, bAdd
            Else
                Channels(intX).RemoveMode strMode
            End If
    End Select
End Sub

Function GetAlias(strChan As String, strData As String) As String
    Dim arrParams() As String, i As Integer, strP As String, strCom As String
    Dim strFinal As String, strAdd As String, bSpace As Boolean, inttemp As Integer
    Dim strTemp As String, strNck As String
    
    If InStr(strData, " ") Then
        Seperate strData, " ", strCom, strData
        arrParams = Split(strData, " ")
    Else
        strCom = strData
        arrParams = Split("", "")
    End If
    bSpace = True
    'DoEvents
    
    For i = LBound(arrParams) To UBound(arrParams)
        strP = arrParams(i)
        'MsgBox strP & ":" & i
        strAdd = ""
        If strP = "$+" Then
            strFinal = LeftR(strFinal, 1)
            bSpace = False
        ElseIf Left(strP, 1) = "$" Then
            strAdd = GetVar(strChan, RightR(strP, 1))
        Else
            strAdd = strP
        End If
        
        strFinal = strFinal & strAdd
        If bSpace Then
            strFinal = strFinal & " "
        Else
            bSpace = True
        End If
    Next i
    
    If Len(strFinal) > 0 Then strFinal = LeftR(strFinal, 1)
    
    ReDim arrParams(1) As String
    arrParams = Split(strFinal, " ")
    
    Dim r As String 'return
    Select Case LCase(strCom)
        Case "query", "q"
            strTemp = Combine(arrParams, 2, -1)
            strNck = Combine(arrParams, 1, 1)
            'MsgBox strNck & "~"
            If QueryExists(strNck) Then
                inttemp = GetQueryIndex(strNck)
                Queries(inttemp).PutText strMyNick, strTemp
                r = "PRIVMSG " & strNck & " :" & strTemp
            Else
                inttemp = NewQuery(strNck, "")
                If UBound(arrParams) > 0 Then
                    Queries(inttemp).PutText strMyNick, strTemp
                    r = "PRIVMSG " & strNck & " :" & strTemp
                End If
            End If
                
        Case "msg"
            r = "PRIVMSG " & Combine(arrParams, 1, 1) & " :" & Combine(arrParams, 2, -1)
        Case "me", "action", "describe"
            strTemp = Combine(arrParams, 1, -1)
            r = "PRIVMSG " & strChan & " :" & strAction & "ACTION " & strTemp & strAction
            If Left(strChan, 1) = "#" Then
                inttemp = GetChanIndex(strChan)
                If inttemp = -1 Then Exit Function
                PutData Channels(inttemp).DataIn, strColor & "06" & strMyNick & " " & strTemp
            Else
                inttemp = GetQueryIndex(strChan)
                If inttemp = -1 Then Exit Function
                PutData Queries(inttemp).DataIn, strColor & "06" & strMyNick & " " & strTemp
            End If
        Case "quit", "signoff"
            r = "QUIT :" & Combine(arrParams, 1, -1)
        Case "notice"
            r = "NOTICE " & Combine(arrParams, 1, 1) & " :" & Combine(arrParams, 2, -1)
        Case "raw"
            r = Combine(arrParams, 1, -1)
        Case "nick"
            If Client.sock.State = 0 Then
                strMyNick = Combine(arrParams, 1, 1)
            Else
                r = "NICK " & Combine(arrParams, 1, 1)
            End If
        Case "id"   'identify with nickserv
            r = "PRIVMSG NickServ :IDENTIFY " & Combine(arrParams, 1, 1)
        Case "part"
            strTemp = Combine(arrParams, 1, -1)
            If UBound(arrParams) = 0 Then
                r = "PART " & strChan
                strTemp = strTemp
            Else
                r = "PART " & Combine(arrParams, 1, 1) & " :" & Combine(arrParams, 2, -1)
                strTemp = LeftOf(strTemp, " ")
            End If
            
            inttemp = GetChanIndex(strTemp)
            
            
            On Error Resume Next
            Channels(inttemp).Tag = "PARTNOW"
            
'            MsgBox r
        Case "server"
            strServer = Combine(arrParams, 1, 1)
            If UBound(arrParams) > 0 Then
                strport = Int(Combine(arrParams, 2, 2))
            End If
            Call Client.mnu_File_Disconnect_Click
            TimeOut 0.1
            Call Client.mnu_File_Connect_Click
        Case "join", "j"
            'ok here's how it is, you can type /join #blah (key), #blah2, #blah3, #blah4 (key),
            'so we need special equiptment to handle this
            Dim strChans() As String
            strChans = Split(strData, ",")
            
            For inttemp = LBound(strChans) To UBound(strChans)
                Dim prefix As String
                prefix = ""
                If Left(strChans(inttemp), 1) <> "#" And Left(strChans(inttemp), 1) <> "&" Then prefix = "#"
                Client.SendData "JOIN " & prefix & strChans(inttemp)
                TimeOut 0.8
            Next inttemp
        Case "connect"
            If Combine(arrParams, 1, 1) <> "" Then
                strServer = Combine(arrParams, 1, 1)
            End If
            If UBound(arrParams) > 0 Then
                strport = Int(Combine(arrParams, 2, 2))
            End If
            Call Client.mnu_File_Disconnect_Click
            Call Client.mnu_File_Connect_Click
        Case "disconnect"
            Call Client.mnu_File_Disconnect_Click
        Case "bl"
            If BuddyList.Visible Then
                Unload BuddyList
            Else
                Load BuddyList
            End If
        Case "kill"
            r = "KILL " & Combine(arrParams, 1, 1) & " :" & Combine(arrParams, 2, -1)
        Case "es"
            PutText Status.DataIn, Combine(arrParams, 1, -1)
        Case "list"
            ChannelsList.lvChannels.ListItems.Clear
            If Combine(arrParams, 1, 1) = "" Then
                r = "LIST >0"
            Else
                r = "LIST " & Combine(arrParams, 1, -1)
            End If
        Case "away"
            If Combine(arrParams, 1, -1) = "" Then
                r = "AWAY :"
                If lngGoneFor = 0 Then Else lngGoneFor = CTime - lngGoneFor
                
                '* Announce?
                If bAnnounce Then
                    ActionAll "is back, gone for " & Duration(lngGoneFor)
                End If
                lngGoneFor = 0
            Else
                r = "AWAY :" & Combine(arrParams, 1, -1)
                lngGoneFor = CTime
                
                '* Announce?
                If bAnnounce Then
                    ActionAll "is away, " & Combine(arrParams, 1, -1)
                End If
            End If
        Case "back"
                r = "AWAY :"
                If lngGoneFor = 0 Then Else lngGoneFor = CTime - lngGoneFor
                
                '* Announce?
                If bAnnounce Then
                    ActionAll "is back, gone for " & Duration(lngGoneFor)
                End If
                lngGoneFor = 0
        Case "send", "s"
            If Combine(arrParams, 1, -1) = "" Then Exit Function
            r = "PRIVMSG " & strChan & " :" & Combine(arrParams, 1, -1)
            If Left(strChan, 1) = "#" Then
                inttemp = GetChanIndex(strChan)
                Channels(inttemp).PutText strMyNick, Combine(arrParams, 1, -1)
            ElseIf strChan = "" Then
            Else
                inttemp = GetQueryIndex(strChan)
                Queries(inttemp).PutText strMyNick, Combine(arrParams, 1, -1)
            End If
        Case "version", "ver"
            r = "PRIVMSG " & Combine(arrParams, 1, 1) & " :" & strAction & "VERSION" & strAction
            EchoActive "[CTCP!" & Combine(arrParams, 1, 1) & "] -> VERSION", 4
        Case "ping", "png"
            r = "PRIVMSG " & Combine(arrParams, 1, 1) & " :" & strAction & "PING " & GetTickCount & strAction
            EchoActive "[CTCP!" & Combine(arrParams, 1, 1) & "] -> PING", 4
        Case "ctcp"
            r = "PRIVMSG " & Combine(arrParams, 1, 1) & " :" & strAction & Combine(arrParams, 2, -1) & strAction
            EchoActive "[CTCP!" & Combine(arrParams, 1, 1) & "] -> " & Combine(arrParams, 2, -1), 4
        Case "kick"
            r = "KICK " & Combine(arrParams, 1, 1) & " " & Combine(arrParams, 2, 2) & " :" & Combine(arrParams, 3, -1)
        Case "k"
            r = "KICK " & strChan & " " & Combine(arrParams, 1, 1) & " :" & Combine(arrParams, 2, -1)
        Case "asctime"
            r = ""
            On Error Resume Next
            PutData Status.DataIn, strColor & "10" & strBold & Combine(arrParams, 1, 1) & strBold & " -> " & strBold & AscTime(CLng(Combine(arrParams, 1, 1))) & strBold
            Exit Function
        Case "m"
            r = "MODE " & strChan & " " & Combine(arrParams, 1, -1)
        Case "clear"
            r = ""
            On Error Resume Next
            Client.ActiveForm.DataIn.Text = ""
            Exit Function
        Case "test"
            MsgBox Duration(Combine(arrParams, 1, 1))
        Case Else
            r = strCom & " " & Combine(arrParams, 1, -1)
    End Select
    
    GetAlias = r
    
End Function

Function SV(lngWhat As Long, strText As String) As String
    If lngWhat = 0 Then
        SV = ""
    ElseIf lngWhat = 1 Then
        SV = lngWhat & " " & strText & " "
    Else
        SV = lngWhat & " " & strText & "s" & " "
    End If
End Function

Sub TimeOut(Duration)
    StartTime = Timer
    Do While Timer - StartTime < Duration
        x = DoEvents()
    Loop
End Sub

Function GetChanIndex(strName As String) As Integer
    Dim i As Integer

    For i = 1 To intChannels
        If LCase(Channels(i).strName) = Replace(LCase(strName), Chr(13), "") Then
            GetChanIndex = i
            Exit Function
        End If
    Next i
    GetChanIndex = -1
End Function

Function GetQueryIndex(strNick As String) As Integer
    Dim i As Integer

    For i = 1 To intQueries
        If LCase(Queries(i).strNick) = LCase(strNick) Then
            GetQueryIndex = i
            Exit Function
        End If
    Next i
    GetQueryIndex = -1
End Function

Function GetVar(strChan As String, strName As String)
    Dim r As String     'r is the return value
    Dim inttemp As String
    
    On Error Resume Next
    Select Case LCase(strName)
        Case "version"
            r = App.Major & "." & App.Minor & App.Revision
        Case "chan", "channel", "ch"
            r = strChan
        Case "me"
            r = strMyNick
        Case "server"
            r = Client.sock.RemoteHost
        Case "port"
            r = Client.sock.RemotePort
        Case "randnick"
            inttemp = GetChanIndex(strChan)
            If Left(strChan, "1") = "#" Then
                With Channels(inttemp)
                    Randomize
                    r = .GetNick(Int(Rnd * .intNicks) + 1)
                End With
            End If
        Case "date"
            r = Date
        Case "time"
            r = Time
        Case "usercount", "users", "unum", "ucount", "chancount"
            inttemp = GetChanIndex(strChan)
            If inttemp = -1 Then
                r = 0
            Else
                r = Channels(inttemp).intUserCount
            End If
        Case "now"
            r = Now
        Case "ctime"
            r = CTime
        Case "ticks"
            r = GetTickCount
    End Select
    GetVar = r
End Function

Function LeftOf(strData As String, strDelim As String) As String
    Dim intPos As Integer
    
    intPos = InStr(strData, strDelim)
    If intPos Then
        LeftOf = Left(strData, intPos - 1)
    Else
        LeftOf = strData
    End If
End Function

Function LeftR(strData As String, intMin As Integer)
    
    On Error Resume Next
    LeftR = Left(strData, Len(strData) - intMin)
End Function

Function NewChannel(strName As String) As Integer
    Dim i As Integer

    For i = 1 To intChannels
        If Channels(i).strName = "" Then
            Channels(i).strName = strName
            Channels(i).Visible = True
            Channels(i).lblName = strName
            Channels(i).Tag = i
            Channels(i).Update
            NewChannel = i
            Exit Function
        End If
    Next i
    intChannels = intChannels + 1
    Set Channels(intChannels) = New Channel
    Channels(intChannels).strName = strName
    Channels(intChannels).Visible = True
    Channels(intChannels).Update
    Channels(intChannels).lblName = strName
    Channels(intChannels).Caption = strName
    
    Channels(intChannels).Tag = intChannels
    NewChannel = intChannels
End Function
Function NewQuery(strNick As String, strHost As String) As Integer
    Dim i As Integer, strHostX As String
    strHostX = RightOf(strHost, "!")
    
    i = GetQueryIndex(strNick)
    If i <> -1 Then
        Queries(i).SetFocus
        Exit Function
    End If

    For i = 1 To intQueries
        If Queries(i).strNick = "" Then
            Queries(i).Caption = strNick
            Queries(i).lblNick = strNick
            Queries(i).strNick = strNick
            Queries(i).strHost = strHostX
            Queries(i).lblHost = strHostX
            Queries(i).Visible = True
            Queries(i).Tag = i
            NewQuery = i
            Exit Function
        End If
    Next i
    
    intQueries = intQueries + 1
    Set Queries(intQueries) = New Query
    Queries(intQueries).strNick = strNick
    Queries(intQueries).lblNick = strNick
    Queries(intQueries).Caption = strNick
    Queries(intQueries).lblHost = strHostX
    Queries(intQueries).strHost = strHostX
    Queries(intQueries).Visible = True
    Queries(intQueries).Tag = intQueries
    NewQuery = intQueries
End Function

Sub NickQuit(strNick As String, strMsg As String)
    For i = 1 To intChannels
        If Channels(i).InChannel(strNick) And Channels(i).strName <> "" Then
            Channels(i).RemoveNick strNick
            PutData Channels(i).DataIn, strColor & "02" & strBold & strNick & strBold & " has Quit IRC [ " & strMsg & " ]"
        End If
    Next i

    For i = 1 To intQueries
        If LCase(Queries(i).strNick) = LCase(strNick) Then
            PutData Queries(i).DataIn, strColor & "02" & strBold & strNick & strBold & " has Quit IRC [ " & strMsg & " ]"
            Exit Sub
        End If
    Next i
End Sub

Function params(parsed As ParsedData, intStart As Integer, intEnd As Integer) As String
    '* This returns the given parameter range as one string
    '* -1 specified as strEnd means from intStart to the last parameter
    '* Ex: strParams(1) is "#mIRC", then params 2 thru 16 are the nicks
    '*     of the users in the channel, simply do something like this
    '*     strNames = Params(2, -1)
    '*     that would return all the nicks into one string, similar to mIRC's
    '*      $#- identifier ($2-) in this case
    
    '* Declare variables
    Dim strFinal As String, intLast As Integer, i As Integer
    
    '* check for bad parameters
    If intStart < 1 Or intEnd > parsed.intParams Then Exit Function
    
    '* if intEnd = -1 then set it to param count
    If intEnd = -1 Then intLast = parsed.intParams Else intLast = intEnd
        
    For i = intStart To intLast
        strFinal = strFinal & parsed.strParams(i)
        If i <> intLast Then strFinal = strFinal & " "
    Next i
    
    params = Replace(strFinal, vbCr, "")
    strFinal = ""
End Function

Sub ParseData(ByVal strData As String, ByRef parsed As ParsedData)

    '* Declare variables
    Dim strTMP As String, i As Integer
    
    '* Reset variables
    bHasPrefix = False
    parsed.strNick = ""
    parsed.strIdent = ""
    parsed.strHost = ""
    parsed.strCommand = ""
    parsed.intParams = 1
    ReDim parsed.strParams(1 To 1) As String
    
    '* Check for prefix, if so, parse nick, ident and host (or just host)
    If Left(strData, 1) = ":" Then
        bHasPrefix = True
        strData = Right(strData, Len(strData) - 1)
        '* Put data left of " " in strHost, data right of " "
        '* into strData
        Seperate strData, " ", parsed.strHost, strData
        parsed.strFullHost = parsed.strHost
        
        '* Check to see if client host name
        If InStr(parsed.strHost, "!") Then
            Seperate parsed.strHost, "!", parsed.strNick, parsed.strHost
            Seperate parsed.strHost, "@", parsed.strIdent, parsed.strHost
        End If
    End If
    
    '* If any params, parse
    If InStr(strData, " ") Then
        Seperate strData, " ", parsed.strCommand, strData
        
        parsed.AllParams = strData
       '* Let's parse all the parameters.. yummy
Begin: '* OH NO I USED A LABEL!

        '* If begginning of param is :, indicates that its the last param
        If Left(strData, 1) = ":" Then
            parsed.strParams(parsed.intParams) = Right(strData, Len(strData) - 1)
            GoTo Finish
        End If
        '* If there is a space still, there is more params
        If InStr(strData, " ") Then
            Seperate strData, " ", parsed.strParams(parsed.intParams), strData
            parsed.intParams = parsed.intParams + 1
            ReDim Preserve parsed.strParams(1 To parsed.intParams) As String
            GoTo Begin
        Else
            parsed.strParams(parsed.intParams) = strData
        End If
    Else
        '* No params, strictly command
        parsed.intParams = 0
        parsed.strCommand = strData
    End If
Finish:
End Sub

Sub ParseMode(strChannel As String, strData As String)
    Dim strModes() As String, strChar As String
    Dim i As Integer, intParam As Integer
    Dim bAdd As Boolean
    
    bAdd = True
    strModes = Split(strData, " ")
    For i = 1 To Len(strModes(0))
        strChar = Mid(strModes(0), i, 1)
        Select Case strChar
            Case "+"
                bAdd = True
            Case "-"
                bAdd = False
            Case "v", "b", "o", "h", "k", "l"
                intParam = intParam + 1
                DoMode strChannel, bAdd, strChar, strModes(intParam)
            Case Else
                DoMode strChannel, bAdd, strChar, ""
        End Select
    Next i
End Sub

Sub PutData(RTF As RichTextBox, strData As String)

    
    'MsgBox InStr(strData, "W")
    If 0 = InStr(strData, strBold) And _
       0 = InStr(strData, strUnderline) And _
       0 = InStr(strData, strReverse) And _
       0 = InStr(strData, strColor) Then
       RTF.SelStart = Len(RTF.Text)
       RTF.SelColor = lngForeColor
       RTF.SelBold = False
       RTF.SelUnderline = False
       RTF.SelStrikeThru = False
       RTF.SelFontName = strFontName
       RTF.SelFontSize = intFontSize
       RTF.SelText = " " & strData & vbCrLf
       Exit Sub
    End If
    
    If strData = "" Then Exit Sub
    'DoEvents
    Dim i As Long, Length As Integer, strChar As String, strBuffer As String
    strData = " " & strData
    Length = Len(strData)
    i = 1
    RTF.SelStart = Len(RTF.Text)
    RTF.SelColor = lngForeColor
    RTF.SelBold = False
    RTF.SelUnderline = False
    RTF.SelStrikeThru = False
    RTF.SelFontName = strFontName
    RTF.SelFontSize = intFontSize
    Do
        strChar = Mid(strData, i, 1)
        Select Case strChar
            Case strBold, Chr(15)
                RTF.SelStart = Len(RTF.Text)
                RTF.SelText = strBuffer
                strBuffer = ""
                RTF.SelBold = Not RTF.SelBold
                i = i + 1
            Case strUnderline
                RTF.SelStart = Len(RTF.Text)
                RTF.SelText = strBuffer
                strBuffer = ""
                RTF.SelUnderline = Not RTF.SelUnderline
                i = i + 1
            Case strReverse
                RTF.SelStart = Len(RTF.Text)
                RTF.SelText = strBuffer
                strBuffer = ""
                RTF.SelStrikeThru = Not RTF.SelStrikeThru
                i = i + 1
            Case strColor
                RTF.SelStart = Len(RTF.Text)
                RTF.SelText = strBuffer
                strBuffer = ""
                i = i + 1
                If i > Length Then GoTo TheEnd
                Do Until Not ValidColorCode(strBuffer) Or i > Length
                    strBuffer = strBuffer & Mid(strData, i, 1)
                    i = i + 1
                Loop
                If ValidColorCode(strBuffer) And i > Length Then GoTo TheEnd
                strBuffer = LeftR(strBuffer, 1)
                RTF.SelStart = Len(RTF.Text)
                If strBuffer = "" Then
                    RTF.SelColor = lngForeColor
                Else
                    RTF.SelColor = AnsiColor(LeftOf(strBuffer, ","))
                End If
                i = i - 1
                strBuffer = ""
            Case Else
                strBuffer = strBuffer & strChar
                i = i + 1
        End Select
    Loop Until i > Length
    If strBuffer <> "" Then
            RTF.SelStart = Len(RTF.Text)
            RTF.SelText = strBuffer
            strBuffer = ""
    End If
TheEnd:
    RTF.SelBold = False
    RTF.SelUnderline = False
    RTF.SelStrikeThru = False
    RTF.SelStart = Len(RTF.Text)
    RTF.SelText = vbCrLf
    
End Sub
Function QueryExists(strNick As String) As Boolean
    Dim i As Integer

    For i = 1 To intQueries
        If LCase(Queries(i).strNick) = LCase(strNick) Then
            QueryExists = True
            Exit Function
        End If
    Next i
    QueryExists = False
End Function

Function RealNick(strNick As String) As String
    strNick = Replace(strNick, "@", "")
    strNick = Replace(strNick, "%", "")
    strNick = Replace(strNick, "+", "")
    RealNick = strNick
End Function

Sub RefreshList(lstBox As ListBox)
    'lstBox.AddItem "", 0
    'lstBox.RemoveItem 0
End Sub

Function RightOf(strData As String, strDelim As String) As String
    Dim intPos As Integer
    intPos = InStr(strData, strDelim)
    
    If intPos Then
        RightOf = Mid(strData, intPos + 1, Len(strData) - intPos)
    Else
        RightOf = strData
    End If
End Function


Function RightR(strData As String, intMin As Integer)
    On Error Resume Next
    RightR = Right(strData, Len(strData) - intMin)
End Function

Sub Seperate(strData As String, strDelim As String, ByRef strLeft As String, ByRef strRight As String)
    '* Seperates strData into 2 variables based on strDelim
    '* Ex: strData is "Bill Clinton"
    '*     Dim strFirstName As String, strLastName As String
    '*     Seperate strData, " ", strFirstName, strLastName
    
    Dim intPos As Integer
    intPos = InStr(strData, strDelim)
    
    If intPos Then
        strLeft = Left(strData, intPos - 1)
        strRight = Mid(strData, intPos + 1, Len(strData) - intPos)
    Else
        strLeft = strData
        strRight = strData
    End If
End Sub


Function ValidColorCode(strCode As String) As Boolean
    'MsgBox strCode
    Dim c1 As Integer, c2 As Integer
    If strCode Like "" Or _
       strCode Like "#" Or _
       strCode Like "##" Or _
       strCode Like "#,#" Or _
       strCode Like "##,#" Or _
       strCode Like "#,##" Or _
       strCode Like "#," Or _
       strCode Like "##," Or _
       strCode Like "##,##" Or _
       strCode Like ",#" Or _
       strCode Like ",##" Then
        Dim strCol() As String
        strCol = Split(strCode, ",")
        'DoEvents
        If UBound(strCol) = -1 Then
            ValidColorCode = True
        ElseIf UBound(strCol) = 0 Then
            If strCol(0) = "" Then strCol(0) = 0
            If CInt(strCol(0)) >= 0 And CInt(strCol(0)) < 16 Then
                ValidColorCode = True
            Else
                ValidColorCode = False
            End If
        Else
            If strCol(0) = "" Then strCol(0) = lngForeColor
            If strCol(1) = "" Then strCol(1) = 0
            c1 = CInt(strCol(0))
            c2 = CInt(strCol(1))
            If c2 < 0 Or c2 > 16 Then
                ValidColorCode = False
            Else
                ValidColorCode = True
            End If
        End If
        ValidColorCode = True
    Else
        ValidColorCode = False
    End If
End Function


