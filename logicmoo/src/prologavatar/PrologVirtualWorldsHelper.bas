Attribute VB_Name = "PrologVirtualWorldsHelper"
Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)


'OnTell event type constants
Public Function int_tell(ByVal inttCtype)
    Select Case inttCtype
        Case 0
                int_tell = "say"
        Case 1
                int_tell = "emote"
        Case 2
                int_tell = "think"
        Case 3
                int_tell = "whisper"
        Case 4
                int_tell = "announce"
        Case 5
                int_tell = "gesture"
        Case 6
                int_tell = "shout"
        Case 7
                int_tell = "report"
        Case 8
                int_tell = "action"
        Case Else
                int_tell = "tellUnknown"
    End Select
End Function

'Other UI Event constants used by this page
Public Function int_uitype(ltype)
    Select Case ltype
        Case 0
            int_uitype = "uiNone"
        Case 1
            int_uitype = "uiLogOff"
        Case 2
            int_uitype = "uiWelcome"
        Case 3
            int_uitype = "uiEvent"
        Case 4
            int_uitype = "uiMUDReport"
        Case Else
            int_uitype = "uiUnknown"
    End Select
End Function


'Report type constants
Public Function int_reporttype(ByVal ltype)
    Select Case ltype
        Case -1
            int_reporttype = "vwErrorText"
        Case 0
            int_reporttype = "vwPrintText"
        Case 1
            int_reporttype = "vwEchoText"
        Case 2
            int_reporttype = "vwEvalText"
        Case 3
            int_reporttype = "vwPrompt"
        Case 4
            int_reporttype = "vwPerfLog"
        Case 5
            int_reporttype = "vwPage"
        Case Else
            int_reporttype = "vwUnkwon"
    End Select
End Function

Public Function Ignored(ByVal BSTR) As Boolean
    Ignored = True
   'If Parent.FilterEx = False Then Ignored = False: Exit Function
    If InStr(BSTR, "Multimedia") Then Exit Function
    If InStr(BSTR, "rror") Then Exit Function
    If InStr(BSTR, "xcept") Then Exit Function
    Ignored = False
End Function

